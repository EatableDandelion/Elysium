#pragma once
#include "RenderingPass.h"

namespace Elysium
{
	struct Light
	{
		public:
			virtual void updateShader(Shader& shader,
									  const Circe::Mat44& viewProjection)=0;
	};

	class DirectionalLight : public Light
	{
		public:
			DirectionalLight(const Circe::Vec3& direction,
							 const Circe::Vec3& hue,
							 const float intensity);

			virtual void updateShader(Shader& shader, 
									  const Circe::Mat44& viewProjection);

			void setDirection(const Circe::Vec3& direction);

			void setHue(const Circe::Vec3& hue);

			void setIntensity(const float intensity);

		private:
			Circe::Vec3 m_direction;
			Circe::Vec3 m_hue;
	};

	class PointLight : public Light
	{
		public:
			PointLight(const Circe::Vec3& position,
					   const Circe::Vec3& hue,
					   const Circe::Vec3& attenuation);

			virtual void updateShader(Shader& shader, 
									  const Circe::Mat44& viewProjection);

			void setPosition(const Circe::Vec3& position);

			void setHue(const Circe::Vec3& hue);

			void setAttenuation(const Circe::Vec3& attenuation);

		private:
			Circe::Vec3 m_hue;
			Circe::Vec3 m_attenuation;
			Circe::Transform3 m_transform;
	};
	
	template<typename L>
	class LightPass : public RenderingPass
	{
		public:
			LightPass(const Mesh& lightMesh): m_lightMesh(lightMesh)
			{}

			virtual void init(Renderer& renderer, Shader& shader)
			{
				shader.updateUniform("gPosition", Texture_Map::POSITION);
				shader.updateUniform("gNormal", Texture_Map::NORMAL);
				shader.updateUniform("gDiffuse", Texture_Map::DIFFUSE);
			}

			virtual void draw(Renderer& renderer, Shader& shader)
			{
				
				renderer.disable(Renderer::DEPTH_WRITE);
				renderer.enable(Renderer::DEPTH_READ);
				renderer.enable(Renderer::FACE_CULLING);
//				glEnable(GL_STENCIL_TEST);
//				glStencilFunc(GL_NOTEQUAL, 0, 0xFF);				
//				glCullFace(GL_BACK);
				shader.updateUniform("eyePosition",  renderer.getCamera()
													 .getTransform()
													 ->getPosition());
				shader.updateUniform("screenSize", 
									 renderer.getScreenSize());

				for(std::shared_ptr<L> light : lights)
				{
					light->updateShader(shader, 
								renderer.getCamera().getViewProjection());

					m_lightMesh.draw();
				}
				
//				glDisable(GL_STENCIL_TEST);
//				glCullFace(GL_FRONT);
				renderer.disable(Renderer::DEPTH_READ);
				renderer.enable(Renderer::FACE_CULLING);
			}

			void addLight(std::shared_ptr<L> light)	 
			{
				lights.push_back(light);
			}

		private:
			Mesh m_lightMesh;
			std::vector<std::shared_ptr<L>> lights;
	};
	
	class AmbientPass : public RenderingPass
	{
		public:
			AmbientPass(const Mesh& screen, const float intensity);

			virtual void init(Renderer& renderer, Shader& shader);

			virtual void draw(Renderer& renderer, Shader& shader);

			void setIntensity(const float intensity);

		private:
			Mesh m_screen;
			float m_intensity;
	};

	class StencilPass : public RenderingPass
	{
		public:
			StencilPass(const Mesh& mesh);

			virtual void init(Renderer& renderer, Shader& shader);

			virtual void draw(Renderer& renderer, Shader& shader);

			void addLight(const std::shared_ptr<Light> light);

		private:
			Mesh m_mesh;
			std::vector<std::shared_ptr<Light>> m_lights;
	};
}
