#pragma once
#include <GL/glew.h>
#include <Circe/Circe.h>
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Elysium
{
	class MeshLoader;

	struct Vertex
	{		
		float x, y, z; 			// Position
		float nx, ny, nz; 		// Normal
		float u, v;				// Texture
	};

	enum MeshType{TRIANGLE_RENDERING = GL_TRIANGLES, WIRE_RENDERING = GL_LINES};

	class Mesh
	{
		public:
			Mesh();
			
			void draw();
			
			void setRenderingType(const MeshType& meshType);

		private:
			unsigned int m_mesh;
			GLenum m_renderType;
			GLuint m_vao;
			int m_drawCount;
			GLuint m_vertexBuffer;
			GLuint m_indexBuffer;
			
			friend class MeshLoader;
	};

	struct MeshData
	{
		std::vector<Vertex> vertices; 
		std::vector<unsigned int> indices;
	};

	struct TempVertex
	{
		float x, y, z;
		float nx, ny, nz;
		float u, v;
		unsigned int normalCount=0;
		unsigned int textCount=0;
	};

	class OBJLoader
	{
		public:
			MeshData load(const std::string& fileName);
	};
	
	class MeshLoader
	{
		public:
			Mesh load(const std::string& fileName);
			
			void unload(Mesh& mesh);
			
			Mesh createMesh(const MeshData& meshData);
	};

	
}
