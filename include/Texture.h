#pragma once
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <cassert>
#include <unordered_map>
#include <vector>
#include <Circe/Circe.h>

namespace Elysium
{
	enum Texture_Map
	{
		POSITION,
		DIFFUSE,
		NORMAL,
		HEIGHT,
		EMISSIVE,
		OPACITY,
		SPECULAR,

		NB_TEXTURE_MAPS
	};

	class Texture
	{
		public:
			Texture();

			Texture(const GLuint& id);

			Texture(const Texture& other);
	
			Texture& operator=(const Texture& other);

			void bind(const unsigned int unit);

			void setType(const Texture_Map type);

			GLuint getHandle() const;

		private:
			GLuint m_texture;
			int width, height;

			friend class TextureLoader;
	};

	class TextureLoader
	{
		public:
			Texture load(const std::string& fileName) const;
			
			void unload(Texture& texture) const;

			Texture newTexture(const int width, const int height) const;	
	};


	class Material
	{
		public:
			void bind();

			void addTexture(const Texture_Map& map, 
							const Texture& texture);

		private:
			std::unordered_map<Texture_Map, Texture> textures;
			unsigned int index[NB_TEXTURE_MAPS];
	};

	class FrameBuffer
	{
		public:
			FrameBuffer(const int width, const int height, 
						const int nbTextures);

			~FrameBuffer();

			void read();

			void write();

		private:
			GLuint m_fbo;
			GLuint m_rbo;
			std::vector<Texture> m_textures;
			TextureLoader m_textureLoader;
	};
}









