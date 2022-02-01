#pragma once
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <cassert>
#include <unordered_map>

namespace Elysium
{
	enum Texture_Map
	{
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
			Texture(const int& width, const int& height, const GLuint& id);

			void bind(const unsigned int unit);

			void setType(const Texture_Map type);

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
}
