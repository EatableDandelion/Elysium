#include "Rendering/Mesh.h"

namespace Elysium
{
	Vertex::Vertex():Vertex(0.0f, 0.0f, 0.0f)
	{}

	Vertex::Vertex(const float x, const float y, const float z,
			   	   const float u, const float v,
			   	   const float nx, const float ny, const float nz):					x(x), y(y), z(z), u(u), v(v), nx(nx), ny(ny), nz(nz)
	{}	


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
				float x,y,z;
				fscanf(file, "%f %f %f\n", &x, &y, &z);
				vertex.x = x;
				vertex.y = y;
				vertex.z = z;
				vertex.u = 0;
				vertex.v = 0;
				vertex.nx = 0;
				vertex.ny = 0;
				vertex.nz = 0;
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

	Mesh MeshLoader::createMesh(const MeshData& meshData) const
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



	MeshData MeshLoader::calculateNormals(const MeshData& mesh) const
	{
		MeshData newMesh;
		newMesh.indices = mesh.indices;

		std::vector<TempVertex> vertices;

		for(int i = 0; i<mesh.vertices.size(); i++)
		{
			TempVertex vertex;

			vertex.x = mesh.vertices.at(i).x;
			vertex.y = mesh.vertices.at(i).y;
			vertex.z = mesh.vertices.at(i).z;

			vertex.nx = 0.0f;
			vertex.ny = 0.0f;
			vertex.nz = 0.0f;

			vertex.u = mesh.vertices.at(i).u;
			vertex.v = mesh.vertices.at(i).v;

			vertices.push_back(vertex);
		}

		for(int i = 0; i<mesh.indices.size(); i+=3)
		{
			TempVertex v1 = vertices[mesh.indices.at(i)];
			TempVertex v2 = vertices[mesh.indices.at(i+1)];
			TempVertex v3 = vertices[mesh.indices.at(i+2)];

			float l1x = v2.x - v1.x;
			float l1y = v2.y - v1.y;
			float l1z = v2.z - v1.z;

			float l2x = v3.x - v1.x;
			float l2y = v3.y - v1.y;
			float l2z = v3.z - v1.z;

			float nx = l1y*l2z - l1z*l2y;	
			float ny = l1z*l2x - l1x*l2z;	
			float nz = l1x*l2y - l1y*l2x;	

			v1.nx = nx;
			v1.ny = ny;
			v1.nz = nz;
			v1.normalCount++;

			v2.nx = nx;
			v2.ny = ny;
			v2.nz = nz;
			v2.normalCount++;

			v3.nx = nx;
			v3.ny = ny;
			v3.nz = nz;
			v3.normalCount++;
		}

		newMesh.vertices = mesh.vertices;
		for(int i = 0; i<vertices.size(); i++)
		{
			int count = vertices.at(i).normalCount;
			newMesh.vertices[i].nx = vertices.at(i).nx/count;
			newMesh.vertices[i].ny = vertices.at(i).ny/count;
			newMesh.vertices[i].nz = vertices.at(i).nz/count;
		}

		return newMesh;
	}

	
	GeometryLoader::GeometryLoader()
	{
		addGeometry("line", newLine());
		addGeometry("arrow", newArrow());
		addGeometry("sprite", newRectangle(false));
		addGeometry("rectangle", newRectangle(true));
		addGeometry("circle", newCircle());
	}

	Mesh GeometryLoader::load(const std::string& fileName)
	{
		return m_meshLoader.createMesh(m_meshes.at(fileName));
	}

	void GeometryLoader::unload(Mesh& mesh)
	{
		m_meshLoader.unload(mesh);
	}

	void GeometryLoader::addGeometry(const std::string& name, 
									 const MeshData& data)
	{
		m_meshes.insert(std::pair<std::string, MeshData>(name, data));
	}

	MeshData GeometryLoader::newRectangle(const bool& wire) const
	{
		float w = 1.0f;
		float h = 1.0f;

		MeshData data;

		data.vertices.push_back(Vertex(-w,-h,0, 0,0, 0,0,-1));	
		data.vertices.push_back(Vertex( w,-h,0, 1,0, 0,0,-1));	
		data.vertices.push_back(Vertex( w, h,0, 1,1, 0,0,-1));	
		data.vertices.push_back(Vertex(-w, h,0, 0,1, 0,0,-1));

		if(wire)
		{
			data.indices.push_back(0);
			data.indices.push_back(1);
			data.indices.push_back(1);
			data.indices.push_back(2);
			data.indices.push_back(2);
			data.indices.push_back(3);
			data.indices.push_back(3);
			data.indices.push_back(0);
		}
		else
		{
			data.indices.push_back(3);
			data.indices.push_back(2);
			data.indices.push_back(1);
			data.indices.push_back(3);
			data.indices.push_back(1);
			data.indices.push_back(0);
		}

		return data;
	}

	MeshData GeometryLoader::newLine() const
	{
		MeshData data;

		data.vertices.push_back(Vertex(-0.5,0.0,0.0, 1));	
		data.vertices.push_back(Vertex( 0.5,0.0,0.0, 1));	

		data.indices.push_back(0);
		data.indices.push_back(1);

		return data;
	}
	
	MeshData GeometryLoader::newArrow() const
	{
		MeshData data;

		data.vertices.push_back(Vertex(-0.5, 0.0,0.0, 1));	
		data.vertices.push_back(Vertex( 0.5, 0.0,0.0, 1));	
		data.vertices.push_back(Vertex( 0.4, 0.1,0.0, 1));	
		data.vertices.push_back(Vertex( 0.4,-0.1,0.0, 1));	

		data.indices.push_back(0);
		data.indices.push_back(1);
		data.indices.push_back(1);
		data.indices.push_back(2);
		data.indices.push_back(1);
		data.indices.push_back(3);

		return data;
	}
	
	MeshData GeometryLoader::newCircle() const
	{
		MeshData data;
		int n = 21;
		Real theta = 0.0;

		for(int i = 0; i < n; i++)
		{
			data.vertices.push_back(Vertex(cos(theta),sin(theta),0.0,1));
			theta += 2.0*3.141593/n;

			data.indices.push_back(i);
			if(i == n-1)
			{
				data.indices.push_back(0);
			}
			else
			{
				data.indices.push_back(i+1);
			}
		}

		return data;	
	}
}

