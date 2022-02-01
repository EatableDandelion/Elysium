#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include "Camera.h"

namespace Elysium
{
	class Shader
	{
		public:
			void bind();

			void updateUniforms(const Circe::Transform3& transform, 
								const Camera& camera);

			void updateUniform(const std::string& name, const float& value);

			void updateUniform(const std::string& name, 
							   const Circe::Vec2& value);

			void updateUniform(const std::string& name, 
							   const Circe::Vec3& value);

			void updateUniform(const std::string& name, 
							   const Circe::Mat44& value);
		
		private:
			GLuint m_program;
			std::vector<GLuint> m_shaderStages;
			std::unordered_map<std::string,GLuint> m_uniforms;
			
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

			void checkShaderError(const GLuint shader, const GLuint flag, 
								  const bool isProgram, 
								  const std::string& errorMessage);
	};
}
