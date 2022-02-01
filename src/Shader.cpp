#include "Shader.h"

namespace Elysium
{
	void Shader::bind()
	{
		glUseProgram(m_program);
	}
			
	void Shader::updateUniforms(const Circe::Transform3& transform,
								const Camera& camera)
	{
		Circe::Mat44 mvp = camera.getViewProjection()
							* transform.getTransformMatrix();

		updateUniform("MVP", mvp);
	}

	void Shader::updateUniform(const std::string& name, const float& value)
	{
		glUniform1f(getUniformLocation(name), value);
	}

	void Shader::updateUniform(const std::string& name, 
							   const Circe::Vec2& value)
	{
		std::vector<float> v({value(0), value(1)});

		glUniform2fv(getUniformLocation(name), 1, &v[0]);
	}

	void Shader::updateUniform(const std::string& name, 
							   const Circe::Vec<3>& value)
	{
		std::vector<float> v({value(0), value(1), value(2)});

		glUniform3fv(getUniformLocation(name), 1, &v[0]);
	}

	void Shader::updateUniform(const std::string& name, 
							   const Circe::Mat44& value)
	{
		std::vector<float> v({value(0,0), value(0,1), value(0,2), value(0,3),
							  value(1,0), value(1,1), value(1,2), value(1,3),
							  value(2,0), value(2,1), value(2,2), value(2,3),
							  value(3,0), value(3,1), value(3,2), value(3,3)});

		glUniformMatrix4fv(getUniformLocation(name), 1, GL_TRUE, &v[0]);
	}

	GLuint Shader::getUniformLocation(const std::string& name)
	{
		if(!m_uniforms.count(name))
		{
			m_uniforms.insert(std::pair<std::string, GLuint>
					(name, glGetUniformLocation(m_program, name.c_str())));
		}
		return m_uniforms.at(name.c_str());
	}


	Shader ShaderLoader::load(const std::string& fileName)
	{
		Shader shader;
		
		GLuint m_program = glCreateProgram();

		shader.m_program = m_program;

		std::vector<GLuint> m_shaderStages;	

		m_shaderStages.push_back(createShader(readShaderFile(fileName+".vs"), 
									 GL_VERTEX_SHADER));
		m_shaderStages.push_back(createShader(readShaderFile(fileName+".fs"), 
									 GL_FRAGMENT_SHADER));


		for(unsigned int i = 0; i < NUM_SHADERS; i++)
			glAttachShader(m_program, m_shaderStages[i]);

		shader.m_shaderStages = m_shaderStages;
		glBindAttribLocation(m_program, 0, "position");
		glBindAttribLocation(m_program, 1, "normal");
		glBindAttribLocation(m_program, 2, "texCoord");

		glLinkProgram(m_program);
		checkShaderError(m_program, GL_LINK_STATUS, true, 
						 "Error: program failed to link");

		glValidateProgram(m_program);
		checkShaderError(m_program, GL_VALIDATE_STATUS, true, 
						 "Error: program is invalid");

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
				output.append(line + "\n");
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

	void ShaderLoader::checkShaderError(const GLuint shader, const GLuint flag, 
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
}

/*
Shader::Shader(const std::string& fileName)
{
	m_program = glCreateProgram();
	
	m_shaders[0] =  CreateShader(LoadShader(fileName+".vs"), 
								 GL_VERTEX_SHADER);
	m_shaders[1] =  CreateShader(LoadShader(fileName+".fs"), 
								 GL_FRAGMENT_SHADER);

	for(unsigned int i = 0; i < NUM_SHADERS; i++)
		glAttachShader(m_program, m_shaders[i]);

	glBindAttribLocation(m_program, 0, "position");
	glBindAttribLocation(m_program, 1, "normal");
	glBindAttribLocation(m_program, 2, "texCoord");

	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, 
					 "Error: program failed to link");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, 
					 "Error: program is invalid");

	//Create uniforms
	m_uniforms[TRANSFORM_U] = glGetUniformLocation(m_program, "transform");

}
*/
