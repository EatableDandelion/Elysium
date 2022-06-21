#include "Rendering/Texture.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

namespace Elysium
{
	Texture::Texture()
	{}

	Texture::Texture(const GLuint& id) : m_texture(id)
	{}

	Texture::Texture(const Texture& other):m_texture(other.m_texture)
	{}

	Texture& Texture::operator=(const Texture& other)
	{
		m_texture = other.m_texture;

		return *this;
	}

	void Texture::bind(const unsigned int unit)
	{
		assert(unit >= 0 && unit <= 31);
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}
		
	GLuint Texture::getHandle() const
	{
		return m_texture;
	}

	Texture TextureLoader::load(const std::string& fileName) const
	{
		int width, height, numComponents;
		unsigned char* imageData = stbi_load(fileName.c_str(), 
											 &width, &height,
											 &numComponents, 4);

		if(imageData == NULL)
		{
			std::cerr << "Error: Texture loading failed: " 
					  << fileName << std::endl;
		}

		GLuint m_texture;
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, 
					 GL_RGBA, GL_UNSIGNED_BYTE, imageData);

		stbi_image_free(imageData);	
//		glBindTexture(GL_TEXTURE_2D, 0);

		return Texture(m_texture);
	}

	void TextureLoader::unload(Texture& texture) const
	{
		glDeleteTextures(1, &(texture.m_texture));
	}

	Texture TextureLoader::newTexture(const int width, const int height) 
																	const
	{
		GLuint id;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
					 GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);

		return Texture(id);
	}

	FrameBuffer::FrameBuffer(const int width, const int height, 
							 const int nbTextures):m_width(width),
												   m_height(height),
												   m_nbTextures(nbTextures),
												   m_open(false)
	{
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		

		for(int i = 0; i < nbTextures; i++)
		{
			m_attachments[i] = GL_COLOR_ATTACHMENT0+i;
			Texture texture = m_textureLoader.newTexture(width, height);
			glFramebufferTexture2D(GL_FRAMEBUFFER, m_attachments[i], 
								   GL_TEXTURE_2D, texture.getHandle(), 0);
			m_textures.push_back(texture);
		}

		glDrawBuffers(nbTextures, m_attachments);

		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 
							  width, height);

		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
								  GL_RENDERBUFFER, m_rbo);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
			CIRCE_ERROR("Framebuffer incomplete.");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	FrameBuffer::~FrameBuffer()
	{
		for(Texture texture : m_textures)
			m_textureLoader.unload(texture);
		glDeleteRenderbuffers(1, &m_rbo);
		glDeleteFramebuffers(1, &m_fbo);
	}

	void FrameBuffer::read()
	{
		m_open = false;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		for(int i = 0; i < m_textures.size(); i++)
		{
			m_textures[i].bind(i);
		}
	}

	void FrameBuffer::write()
	{
		m_open = true;
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		glDrawBuffers(m_nbTextures, m_attachments);
	}

	void FrameBuffer::applyDepthBuffer()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height,
			   			  GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT,
						  GL_NEAREST);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);	
	}

	bool FrameBuffer::isOpen() const
	{
		return m_open;
	}
}





