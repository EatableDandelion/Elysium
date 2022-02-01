#pragma once
#include <Circe/Circe.h>
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "Texture.h"

//Source for model: https://learnopengl.com/Model-Loading/Model

namespace Elysium
{
	struct TexturedMesh
	{
		Mesh m_mesh;
		Material m_material;
	};

	class Model
	{
		public:
			void draw();

		private:
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
