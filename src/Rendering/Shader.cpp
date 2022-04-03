#include "Rendering/Shader.h"

namespace Elysium
{	

	void UniformMap::uploadUniforms(Shader& shader)
	{
		shader.updateUniforms(*this);
	}

	void UniformMap::setUniform(const std::string& name,
								const std::vector<Real> value)
	{
		std::size_t nameID = CIRCE_STRING_ID(name);

		if(hasUniform(nameID))
		{
			unsigned int size = m_positions.at(nameID).size;

			if(size != value.size()) 
				CIRCE_ERROR("The uniform "+name+" has changed size.");

			unsigned int offset = m_positions.at(nameID).offset;
			
			for(int i = 0; i < size; i++)
				m_values[offset + i] = value[i];
		}
		else
		{
			BufferPosition position;

			position.offset = m_values.size();
			position.size 	= value.size();

			m_positions.insert(
				std::pair<std::size_t, BufferPosition>(nameID, position));

			for(int i = 0; i < value.size(); i++)
				m_values.push_back(value[i]);
		}
	}

	void UniformMap::setUniform(const std::string& name, 
								const Real& value)
	{
		setUniform(name, std::vector<Real>({value}));	
	}

	void UniformMap::setUniform(const std::string& name, 
					const Circe::Vec2& value)
	{
		setUniform(name, std::vector<Real>({value(0), value(1)}));	
	}

	void UniformMap::setUniform(const std::string& name, 
					const Circe::Vec3& value)
	{
		setUniform(name, std::vector<Real>({value(0),value(1),value(2)}));
	}

	void UniformMap::setUniform(const std::string& name,const Mat& value)
	{
		std::vector<Real> v(
				{value(0,0),value(0,1),value(0,2),value(0,3),
				 value(1,0),value(1,1),value(1,2),value(1,3),
			     value(2,0),value(2,1),value(2,2),value(2,3),
			     value(3,0),value(3,1),value(3,2),value(3,3)});

		setUniform(name, v);	
	}

	std::vector<Real> UniformMap::operator()(const std::size_t name) const
	{
		std::vector<Real> uniform;

		if(hasUniform(name))
		{	
			unsigned int size = m_positions.at(name).size;
			unsigned int offset = m_positions.at(name).offset;
		

			for(int i = 0; i < size; i++)
				uniform.push_back(m_values[offset + i]);
		}

		return uniform;
	}
			
	bool UniformMap::hasUniform(const std::size_t nameID) const
	{
		return m_positions.count(nameID);
	}

	void Shader::bind()
	{
		glUseProgram(m_program);
	}
		
	void Shader::updateUniforms(const UniformMap& uniformMap)
	{
		for(auto& pair : m_uniforms)
		{
			std::string type = pair.second.type;
			GLuint location = pair.second.location;
			std::vector<Real> floatValue = uniformMap(pair.first);

			std::vector<float> value;
			for(Real real : floatValue)
				value.push_back((float)real);

			if(value.size() == 0)
			{
				bool debug = false;
				if(debug)
				{
					std::string name = pair.second.name;
					CIRCE_ERROR("Uniform "+type+" "+name
								+" has not been set.");
				}
				continue;
			}
			if(type == "float")
					glUniform1f(location, value[0]);
			else if(type == "vec2")
					glUniform2fv(location, 1, &value[0]);
			else if(type == "vec3")
					glUniform3fv(location, 1, &value[0]);
			else if(type == "vec4")
					glUniform4fv(location, 1, &value[0]);
			else if(type == "mat4")
					glUniformMatrix4fv(location, 1, GL_TRUE, &value[0]);
			else if(type == "sampler2D")
					glUniform1i(location, (int)value[0]);
//			else
//					CIRCE_ERROR("Uniform type: "+type
//								+" is not supported.");
		}
	}

	void Shader::updateUniform(const std::string& name, const Real& value)
	{
		glUniform1f(getUniformLocation(name), value);
	}

	void Shader::updateUniform(const std::string& name, 
							   const Circe::Vec2& value)
	{
		std::vector<float> v({(float)value(0), (float)value(1)});

		glUniform2fv(getUniformLocation(name), 1, &v[0]);
	}

	void Shader::updateUniform(const std::string& name, 
							   const Circe::Vec<3>& value)
	{
		std::vector<float> v({(float)value(0), 
							  (float)value(1), 
							  (float)value(2)});

		glUniform3fv(getUniformLocation(name), 1, &v[0]);
	}

	void Shader::updateUniform(const std::string& name,
							   const Texture_Map texture_map)
	{
		glUniform1i(getUniformLocation(name), (int)texture_map);
	}

	void Shader::updateUniform(const std::string& name, 
							   const Mat& value)
	{
		std::vector<float> v;

		for(int i = 0; i<value.getNbRows(); i++)
			for(int j = 0; j<value.getNbCols(); j++)
				v.push_back((float)value(i,j));

		glUniformMatrix4fv(getUniformLocation(name), 1, GL_TRUE, &v[0]);
	}



	GLuint Shader::getUniformLocation(const std::string& name)
	{
		std::size_t nameID = CIRCE_STRING_ID(name);

		if(!m_uniforms.count(nameID))
		{
//			CIRCE_ERROR("Shader does not contain uniform: "+name);
			return 0;
		}

		return m_uniforms.at(nameID).location;
	}
			
	void Shader::addUniform(const std::string& name, 
							const std::string& type)
	{
		std::size_t nameID = CIRCE_STRING_ID(name);
		Uniform uniform;
		uniform.location = glGetUniformLocation(m_program, name.c_str()); 
		uniform.name = name;
		uniform.type = type;

		m_uniforms.insert(std::pair<std::size_t,Uniform>(nameID, uniform));
	}

	Shader ShaderLoader::load(const std::string& fileName)
	{
		Shader shader;
		
		GLuint m_program = glCreateProgram();

		shader.m_program = m_program;

		std::vector<GLuint> shaderStages;	

		std::string extensions[] = {".vs", ".fs"};
		GLuint stageNames[] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};

		std::vector<std::string> shaderTexts;

		for(unsigned int i = 0; i < NUM_SHADERS; i++)
		{
			std::string shaderText = 
							readShaderFile(fileName+extensions[i]);

			shaderStages.push_back(createShader(shaderText,
												stageNames[i]));

			shaderTexts.push_back(shaderText);
		}
		
		for(unsigned int i = 0; i < NUM_SHADERS; i++)
			glAttachShader(m_program, shaderStages[i]);

		shader.m_shaderStages = shaderStages;
		glBindAttribLocation(m_program, 0, "position");
		glBindAttribLocation(m_program, 1, "normal");
		glBindAttribLocation(m_program, 2, "texCoord");

		glLinkProgram(m_program);
		checkShaderError(m_program, GL_LINK_STATUS, true, 
						 "Error: program failed to link");

		glValidateProgram(m_program);
		checkShaderError(m_program, GL_VALIDATE_STATUS, true, 
						 "Error: program is invalid");

		for(std::string shaderText : shaderTexts)
			parseStage(shaderText, shader);

		return shader;
	}

	void ShaderLoader::unload(Shader& shader)
	{
		for(unsigned int i = 0; i < NUM_SHADERS; i++)
		{
			glDetachShader(shader.m_program, shader.m_shaderStages[i]);
			glDeleteShader(shader.m_shaderStages[i]);
		}

		glDeleteProgram(shader.m_program);
	}


	std::string ShaderLoader::readShaderFile(const std::string& fileName)
	{
		std::ifstream file;
		file.open(fileName.c_str());

		std::string output;
		std::string line;

		if(file.is_open())
		{
			while(file.good())
			{
				getline(file, line);

				if(line.find("#include") != std::string::npos)
				{
					std::string incFile = split(line, ' ')[1];
					incFile = incFile.substr(1, incFile.length()-2);
					std::string directory = getDirectory(fileName);
					output.append(readShaderFile(directory+incFile)+ "\n");
				}
				else
				{
					output.append(line + "\n");
				}

			}
		}
		else
		{
			std::cerr << "Unable to load shader " << fileName << std::endl;
		}

		return output;
	}

	GLuint ShaderLoader::createShader(const std::string& text, 
									  const GLenum shaderType)
	{
		GLuint shader = glCreateShader(shaderType);

		if(shader == 0)
			std::cerr << "Error: shader creation failed" << std::endl;

		const GLchar* shaderSourceStrings[1];
		GLint shaderSourceStringLengths[1];
		shaderSourceStrings[0] = text.c_str();
		shaderSourceStringLengths[0] = text.length();

		glShaderSource(shader, 1, shaderSourceStrings, 
					   shaderSourceStringLengths);	

		glCompileShader(shader);

		checkShaderError(shader, GL_COMPILE_STATUS, false, 
						 "Error: shader compilation failed");
		

		return shader;
	}

	void ShaderLoader::checkShaderError(const GLuint shader, 
										const GLuint flag, 
										bool isProgram, 
										const std::string& errorMessage)
	{
		GLint success = 0;
		GLchar error[1024] = {0};

		if(isProgram)
			glGetProgramiv(shader, flag, &success);
		else
			glGetShaderiv(shader, flag, &success);

		if(success == GL_FALSE)
		{
			if(isProgram)
				glGetProgramInfoLog(shader, sizeof(error), NULL, error);
			else
				glGetShaderInfoLog(shader, sizeof(error), NULL, error);

			std::cerr << errorMessage << ": " << error << std::endl;
		}
	}


	void ShaderLoader::parseStage(const std::string& text, 
								  Shader& shader)
	{
		std::istringstream file(text);
		std::string line;
		std::string varName;
		std::string varType;
		while(std::getline(file, line))
		{
			if(parseLine(line, "uniform", varName, varType))
			{
//				CIRCE_INFO("uniform: " + varType + " " + varName);
				shader.addUniform(varName, varType);
			}
		}
	}
	
	bool ShaderLoader::parseLine(const std::string& line, 
								 const std::string& typeName, 
								 std::string& varName, 
								 std::string& varType)
	{
		if(line.find(typeName) == std::string::npos)
		{
			return false;
		}
		
		std::size_t start = line.find(typeName)+typeName.length()+1;
		std::size_t end = line.find(";");
		std::string subline = line.substr(start, end-start);
		
		std::size_t mid = subline.find(" ");
		
		varType = subline.substr(0, mid);
		varName = subline.substr(mid+1,subline.length()-(mid+1));

		return true;
	}

	//Source https://github.com/BennyQBD/3DEngineCpp
	std::vector<std::string> ShaderLoader::split(const std::string& line,
												 const char token)
	{
		std::vector<std::string> res;

		const char* cline = line.c_str();
		int start = 0;
		int end = 0;

		while(end <= line.length())
		{
			while(end <= line.length())
			{
				if(cline[end] == token)
					break;
				end++;
			}
			
			res.push_back(line.substr(start, end-start));
			start = end + 1;
			end = start;
		}

		return res;
	}

	std::string ShaderLoader::getDirectory(const std::string& fullPath)
	{
		std::size_t pos = fullPath.find_last_of('/');
		
		if(pos == std::string::npos)
			CIRCE_ERROR("Could not find directory name from "+fullPath);

		return fullPath.substr(0, pos+1);
	}
}
