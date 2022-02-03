#include "Mesh.h"

namespace Elysium
{
	Mesh::Mesh():m_renderType(TRIANGLE_RENDERING)
	{}

	void Mesh::draw()
	{
		glBindVertexArray(m_vao);
		glDrawElements(m_renderType, m_drawCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Mesh::setRenderingType(const MeshType& meshType)
	{
		m_renderType = meshType;
	}

	MeshData OBJLoader::load(const std::string& fileName)
	{
		MeshData meshData;
		
		std::FILE* file = fopen(fileName.c_str(), "r");
		if(file == NULL)
		{
			CIRCE_ERROR("Could not open "+fileName+" mesh.");
		}
		
		std::vector<TempVertex> vertices;
		std::vector<Circe::Vec<3>> normals;
		std::vector<Circe::Vec<2>> textCoords;
		std::vector<unsigned int> resultIndices;
		bool hasNormals = false;
		bool hasTextures = false;
		
		char lineStart[32];
		int firstWord = fscanf(file, "%s", lineStart);
		while(firstWord != EOF)
		{
			if(strcmp(lineStart, "v")==0)		//Position coords
			{
				TempVertex vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				vertices.push_back(vertex);
			}
			else if(strcmp(lineStart, "vt")==0) //Texture coords
			{
				float u, v;
				fscanf(file, "%f %f\n", &u, &v);
				textCoords.push_back(Circe::Vec<2>(u, v));
				hasTextures = true;
			}
			else if(strcmp(lineStart, "vn")==0) //Normal coords
			{
				float nx, ny, nz;
				fscanf(file, "%f %f %f\n", &nx, &ny, &nz);
				normals.push_back(Circe::Vec<3>(nx, ny, nz));
				hasNormals = true;
			}
			else if(strcmp(lineStart, "f")==0) //Facets
			{
				if(hasTextures && hasNormals)
				{
					unsigned int p[4], u[4], n[4];
					
					int matches = fscanf(file,
						"%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", 
												&p[0], &u[0], &n[0], 
												&p[1], &u[1], &n[1], 
												&p[2], &u[2], &n[2], 
												&p[3], &u[3], &n[3]);

					if(matches == 9 || matches ==12)
					{//It's a triangle or a quad
						for(int i = 0; i<(matches/3); i++)
						{
							p[i]--; u[i]--; n[i]--;

							Circe::Vec<2> uv = textCoords[u[i]];
							Circe::Vec<3> normal = normals[n[i]];

							vertices[p[i]].u += uv(0);
							vertices[p[i]].v += uv(1);
							vertices[p[i]].textCount++;
							
							vertices[p[i]].nx += normal(0);
							vertices[p[i]].ny += normal(1);
							vertices[p[i]].nz += normal(2);
							vertices[p[i]].normalCount++;
						}
						
						resultIndices.push_back(p[0]);
						resultIndices.push_back(p[1]);
						resultIndices.push_back(p[2]);
						if(matches == 12)
						{
							resultIndices.push_back(p[0]);
							resultIndices.push_back(p[2]);
							resultIndices.push_back(p[3]);
						}
						
					}else{
						CIRCE_ERROR("Mesh " + fileName
									+ " format not handled.");
						break;
					}

				}
				else
				{
					CIRCE_ERROR("Mesh "+fileName+" format not handled.");
					break;
				}
			}
			firstWord = fscanf(file, "%s", lineStart);
		}
		
		std::vector<Vertex> resultVertices;
		for(TempVertex vertex : vertices)
		{
			Vertex resultVertex;
			resultVertex.x = vertex.x;
			resultVertex.y = vertex.y;
			resultVertex.z = vertex.z;
			resultVertex.u = vertex.u/vertex.textCount;
			resultVertex.v = vertex.v/vertex.textCount;
			resultVertex.nx = vertex.nx/vertex.normalCount;
			resultVertex.ny = vertex.ny/vertex.normalCount;
			resultVertex.nz = vertex.nz/vertex.normalCount;
			resultVertices.push_back(resultVertex);
		}

		meshData.vertices = resultVertices;
		meshData.indices = resultIndices;
		fclose(file);
		
		return meshData;
	}
	
	Mesh MeshLoader::load(const std::string& fileName)
	{	
		MeshData meshData = OBJLoader().load(fileName);
		Mesh mesh = createMesh(meshData);

		return mesh;
	}

	void MeshLoader::unload(Mesh& mesh)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &(mesh.m_vertexBuffer));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &(mesh.m_indexBuffer));
	}

	Mesh MeshLoader::createMesh(const MeshData& meshData)
	{
		std::vector<Vertex> vertices = meshData.vertices;
		std::vector<unsigned int> indices = meshData.indices;
		
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		
		GLuint vertexBuffer;
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vertices[0]), 
					 &vertices[0], GL_STATIC_DRAW);
			
		GLuint indexBuffer;
		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
					 indices.size()*sizeof(indices[0]), 
					 &indices[0], GL_STATIC_DRAW);	
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8,
			   				 (void*) 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8,
			   			 	 (void*) (sizeof(float) * 3));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 
							 (void*) (sizeof(float) * 6));

		glBindVertexArray(0);

		Mesh mesh;
		mesh.m_vao=vao;
		mesh.m_vertexBuffer = vertexBuffer;
		mesh.m_indexBuffer = indexBuffer;
		mesh.m_drawCount = indices.size();
		
		return mesh;
	}

}

