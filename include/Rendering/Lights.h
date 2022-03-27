#pragma once
#include "Rendering/RenderingPass.h"

namespace Elysium
{
	class DirectionalLight : public Component
	{
		public:
			DirectionalLight(const Vec3& direction,
							 const Vec3& hue,
							 const Real intensity);

			void updateShader(Shader& shader, const Mat& viewProjection);

			void setDirection(const Circe::Vec3& direction);

			void setHue(const Circe::Vec3& hue);

			void setIntensity(const Real intensity);

		private:
			Vec3 m_direction;
			Vec3 m_hue;
	};

	class PointLight : public Component
	{
		public:
			PointLight(const Vec3& position,
					   const Vec3& hue,
					   const Vec3& attenuation);

			void updateShader(Shader& shader, const Mat& viewProjection);

			void setPosition(const Vec3& position);

			void setHue(const Vec3& hue);

			void setAttenuation(const Vec3& attenuation);

		private:
			Vec3 m_hue;
			Vec3 m_attenuation;
			Transform3 m_transform;
	};

	class DirectionalLightPass : public RenderingPass
	{
		public:
			DirectionalLightPass(const Mesh& mesh);

			virtual void init(Renderer& renderer, Shader& shader);

			virtual void draw(std::vector<Entity>& entities,
							  Renderer& renderer, Shader& shader);

		private:
			Mesh m_mesh;
	};
	
	class PointLightPass : public RenderingPass
	{
		public:
			PointLightPass(const Mesh& mesh);

			virtual void init(Renderer& renderer, Shader& shader);

			virtual void draw(std::vector<Entity>& entities,
							  Renderer& renderer, Shader& shader);

		private:
			Mesh m_mesh;
	};

	class AmbientPass : public RenderingPass
	{
		public:
			AmbientPass(const Mesh& screen, 
						const Real intensity);

			virtual void init(Renderer& renderer, Shader& shader);

			virtual void draw(std::vector<Entity>& entities,
							  Renderer& renderer, Shader& shader);

			void setIntensity(const Real intensity);

		private:
			Mesh m_screen;
			Real m_intensity;
	};

/*	class StencilPass : public RenderingPass
	{
		public:
			StencilPass(const Mesh& mesh);

			virtual void init(Renderer& renderer, Shader& shader);

			virtual void draw(Renderer& renderer, Shader& shader);

			void addLight(const std::shared_ptr<Light> light);

		private:
			Mesh m_mesh;
			std::vector<std::shared_ptr<Light>> m_lights;
	};*/
}
