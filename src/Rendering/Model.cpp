#include "Rendering/Model.h"

namespace Elysium
{

	Material::Material()
	{}

	Material::Material(const Material& other):m_textures(other.m_textures)
	{
		for(int i = 0; i<NB_TEXTURE_MAPS; i++)
			index[i] = other.index[i];
	}


	void Material::bind()
	{
		unsigned int unit = 0;
		for(std::pair<Texture_Map, Texture> pair : m_textures)
		{
			pair.second.bind(unit);
			unit++;
		}
	}

	void Material::addTexture(const Texture_Map& map, 
							  const Texture& texture)
	{
		m_textures.insert(std::pair<Texture_Map, Texture>(map, texture));
		index[map]++;
	}


	Model::Model()
	{}

	Model::Model(const Model& model):m_tMeshes(model.m_tMeshes)
	{}

	Model::Model(const Mesh& mesh, const Texture& texture)
	{
		TexturedMesh tMesh;
		tMesh.m_mesh = mesh;
		tMesh.m_material.addTexture(Texture_Map::DIFFUSE, texture);
		m_tMeshes.push_back(tMesh);	
	}

	void Model::draw(Shader& shader)
	{
		shader.updateUniforms(m_register);
		for(TexturedMesh tMesh : m_tMeshes)
		{
			tMesh.m_material.bind();
			tMesh.m_mesh.draw();
		}
	}

	void Model::addMesh(const Mesh& mesh)
	{
		TexturedMesh newMesh;
		newMesh.m_mesh = mesh;
		m_tMeshes.push_back(newMesh);
	}

	Model ModelLoader::load(const std::string& fileName)
	{
		const aiScene *scene = m_importer.ReadFile(fileName, 
					aiProcess_Triangulate | aiProcess_FlipUVs);

		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
				!scene->mRootNode)
		{
			std::cout << "Assimp: " << m_importer.GetErrorString() 
									<< std::endl;
			return Model();
		}

		std::string dir = fileName.substr(0, fileName.find_last_of('/'));

		Model model;
		processNode(scene->mRootNode, scene, model.m_tMeshes, dir);

		return model;
	}


	void ModelLoader::unload(Model& model)
	{
		for(TexturedMesh tMesh : model.m_tMeshes)
			m_meshLoader.unload(tMesh.m_mesh);	
	}

	void ModelLoader::processNode(aiNode* node, const aiScene* scene,
								  std::vector<TexturedMesh>& meshes,
								  const std::string& directory)
	{
		for(unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene, directory));
		}
		for(unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene, meshes, directory);
		}
	}

	TexturedMesh ModelLoader::processMesh(aiMesh* mesh, 
										  const aiScene* scene,
								  		  const std::string& directory)
	{
		MeshData meshData;
		std::vector<Texture> textures;

		/** Process vertices */
		for(unsigned int i = 0; i< mesh->mNumVertices; i++)
		{
			Vertex vertex;

			vertex.x = mesh->mVertices[i].x;
			vertex.y = mesh->mVertices[i].y;
			vertex.z = mesh->mVertices[i].z;

			vertex.nx = mesh->mNormals[i].x;
			vertex.ny = mesh->mNormals[i].y;
			vertex.nz = mesh->mNormals[i].z;

			if(mesh->mTextureCoords[0])
			{
				vertex.u = mesh->mTextureCoords[0][i].x;
				vertex.v = mesh->mTextureCoords[0][i].y;
			}
			else
			{
				vertex.u = 0.0f;
				vertex.v = 0.0f;
			}	

			meshData.vertices.push_back(vertex);
		}

		/** Process indices */
		for(unsigned int i = 0; i< mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for(unsigned int j = 0; j < face.mNumIndices; j++)
				meshData.indices.push_back(face.mIndices[j]);
		}

		TexturedMesh tMesh;
		tMesh.m_mesh = m_meshLoader.createMesh(meshData);
		
		 
		/** Process materials */
		if(mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = 
						scene->mMaterials[mesh->mMaterialIndex];

			loadMaterial(tMesh.m_material, material, 
								aiTextureType_DIFFUSE,
								Texture_Map::DIFFUSE,
								"diffuse_map", directory);
/*
			loadMaterial(tMesh.m_material, material, 
								aiTextureType_NORMALS,
								Texture_Map::NORMAL,
							  	"normal_map", directory);

			loadMaterial(tMesh.m_material, material, 
								aiTextureType_HEIGHT,
								Texture_Map::HEIGHT,
							  	"height_map", directory);

			loadMaterial(tMesh.m_material, material, 
								aiTextureType_EMISSIVE,
								Texture_Map::EMISSIVE,
							  	"emissive_map", directory);

			loadMaterial(tMesh.m_material, material, 
								aiTextureType_SPECULAR,
								Texture_Map::SPECULAR,
								"specular_map", directory);

*/		}
		

		return tMesh;
	}

	void ModelLoader::loadMaterial(Material& material,
								  const aiMaterial* aiMaterial, 
								  const aiTextureType type,
								  const Texture_Map map,
							  	  const std::string typeName,
								  const std::string& directory)
	{
		for(unsigned int i = 0; i < aiMaterial->GetTextureCount(type); i++)
		{
			aiString str;
			aiMaterial->GetTexture(type, i, &str);
			material.addTexture(map, m_textureLoader.
						load(directory+"/"+std::string(str.C_Str())));
		}
	}
}




