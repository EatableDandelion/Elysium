#pragma once
#include <Circe/Circe.h>
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Rendering/Mesh.h"
#include "Rendering/Texture.h"
#include "Rendering/Shader.h"

//Source for model: https://learnopengl.com/Model-Loading/Model

namespace Elysium
{
	class Material
	{
		public:
			Material();

			Material(const Material& other);

			void bind();

			void addTexture(const Texture_Map& map, 
							const Texture& texture);
	
		private:
			std::unordered_map<Texture_Map, Texture> m_textures;
			unsigned int index[NB_TEXTURE_MAPS];
	};

	struct TexturedMesh
	{
		Mesh m_mesh;
		Material m_material;
	};

	class Model
	{
		public:
			Model();

			Model(const Mesh& mesh, const Texture& texture);

			Model(const Model& model);

			void draw(Shader& shader);

			void addMesh(const Mesh& mesh);

			template<typename T>
			void setVariable(const std::string& name, const T& value)
			{
				m_register.setVariable(name, value);
			}

		private:
			Register m_register;
			std::vector<TexturedMesh> m_tMeshes;

			friend class ModelLoader;
	};


	class ModelLoader
	{
		public:
			Model load(const std::string& fileName);

			void unload(Model& model);

		private:
			Assimp::Importer m_importer;
			MeshLoader m_meshLoader;
			TextureLoader m_textureLoader;

			void processNode(aiNode* node, const aiScene* scene,
					 	 std::vector<TexturedMesh>& meshes,
						 const std::string& directory);

			TexturedMesh processMesh(aiMesh* mesh, const aiScene* scene,
									 const std::string& directory);
	
			void loadMaterial(Material& material,
							  const aiMaterial* aiMaterial, 
							  const aiTextureType type,
							  const Texture_Map map,
						  	  const std::string typeName,
							  const std::string& directory);
	};

}
