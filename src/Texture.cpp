#include "Texture.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

namespace Elysium
{
	Texture::Texture()
	{}

	Texture::Texture(const int& width, const int& height, 
					 const GLuint& id) : m_texture(id)
	{}

	void Texture::bind(const unsigned int unit)
	{
		assert(unit >= 0 && unit <= 31);

		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_texture);
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

		return Texture(width, height, m_texture);
	}

	void TextureLoader::unload(Texture& texture) const
	{
		glDeleteTextures(1, &(texture.m_texture));
	}

	void Material::bind()
	{
		unsigned int unit = 0;
		for(std::pair<Texture_Map, Texture> pair : textures)
		{
			pair.second.bind(unit);
			unit++;
		}
	}

	void Material::addTexture(const Texture_Map& map, 
							  const Texture& texture)
	{
		textures.insert(std::pair<Texture_Map, Texture>(map, texture));
		index[map]++;
	}
}





