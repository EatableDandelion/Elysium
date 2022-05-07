#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <GL/glew.h>
#include <Circe/Circe.h>
#include "Game/Camera.h"
#include "Rendering/Texture.h"

namespace Elysium
{
	class Shader
	{
		struct Uniform
		{
			GLuint location;
			std::string name;
			std::string type;
		};

		public:
			void bind();

			void updateUniforms(const Circe::Register& uniformMap);

			void updateUniform(const std::string& name, 
							   const Real& value);

			void updateUniform(const std::string& name, 
							   const Circe::Vec2& value);

			void updateUniform(const std::string& name, 
							   const Circe::Vec3& value);

			void updateUniform(const std::string& name, 
							   const Mat& value);
	
			void updateUniform(const std::string& name, 
							   const Texture_Map value);

			template<std::size_t N, typename dummy = void>
			void updateUniform(const std::string& name,
							   const Real value[N])
			{}

			template<typename dummy>
			void updateUniform(const std::string& name,
							   const Real value[1])
			{
				glUniform1f(getUniformLocation(name), value[0]);
			}

			void addUniform(const std::string& name, 
							const std::string& type);

		private:
			GLuint m_program;
			std::vector<GLuint> m_shaderStages;
			std::unordered_map<std::size_t, Uniform> m_uniforms;

			GLuint getUniformLocation(const std::string& name);

			friend class ShaderLoader;
	};

	class ShaderLoader
	{
		public:
			Shader load(const std::string& fileName);

			void unload(Shader& shader);

		private:
			const int NUM_SHADERS = 2;

			std::string readShaderFile(const std::string& fileName);

			GLuint createShader(const std::string& text, 
								const GLenum shaderType);

			void checkShaderError(const GLuint shader, 
								  const GLuint flag, 
								  bool isProgram, 
								  const std::string& errorMessage);

			void parseStage(const std::string& text, 
						    Shader& shader);

			bool parseLine(const std::string& line, 
						   const std::string& typeName, 
						   std::string& varName, 
						   std::string& varType);

			std::vector<std::string> split(const std::string& line,
										   const char token);
	
			std::string getDirectory(const std::string& fullPath);
	};
}
