#include "Rendering/Lights.h"

namespace Elysium
{

	DirectionalLight::DirectionalLight(const Vec3& direction,
					  				   const Vec3& hue,
									   const Real intensity)
	{
		m_direction = Circe::normalize(direction);
		m_hue = hue*intensity/std::max(hue(0),std::max(hue(1),hue(2)));
	}
							

	void DirectionalLight::updateShader(Shader& shader, 
										const Mat& viewProjection)
	{
		shader.updateUniform("lightColor", m_hue);
		shader.updateUniform("lightDirection", m_direction);
	}

	void DirectionalLight::setDirection(const Vec3& direction)
	{
		m_direction = Circe::normalize(direction);
	}

	void DirectionalLight::setHue(const Vec3& hue)
	{
		Real oldIntensity = std::max(m_hue(0),std::max(m_hue(1),m_hue(2)));
		Real newIntensity = std::max(hue(0),std::max(hue(1),hue(2)));
		m_hue=hue*oldIntensity/newIntensity;
	}

	void DirectionalLight::setIntensity(const Real intensity)
	{
		Real oldIntensity = std::max(m_hue(0),std::max(m_hue(1),m_hue(2)));
		m_hue = m_hue*intensity/oldIntensity;
	}

	PointLight::PointLight(const Vec3& position,
					  	   const Vec3& hue,
						   const Vec3& attenuation)
	{
		setPosition(position);
		setHue(hue);
		setAttenuation(attenuation);
	}
							
	void PointLight::updateShader(Shader& shader, 
								  const Mat& viewProjection)
	{
		shader.updateUniform("lightColor", m_hue);
		shader.updateUniform("lightPosition", m_transform->getPosition());
		shader.updateUniform("lightAttenuation", m_attenuation);

		Mat mvp = viewProjection*m_transform->getTransformMatrix();
		shader.updateUniform("MVP", mvp);
	}

	void PointLight::setPosition(const Vec3& position)
	{
		m_transform->setPosition(position);
	}

	void PointLight::setHue(const Vec3& hue)
	{
		m_hue = hue/std::max(hue(0),std::max(hue(1), hue(2)));
	}

	void PointLight::setAttenuation(const Vec3& attenuation)
	{
		m_attenuation = attenuation;

		Real c = attenuation(0);
		Real l = attenuation(1);
		Real e = attenuation(2);

		Real scale = 0.2f*(-l+std::sqrt(l*l-4.0f*e*(c-256.0f)))/(2.0f*e);
		m_transform->setScale(Vec3(scale));
	}

	DirectionalLightPass::DirectionalLightPass(const Mesh& mesh)
		: m_mesh(mesh)	
	{}

	void DirectionalLightPass::init(Renderer& renderer, Shader& shader)
	{
		shader.updateUniform("gPosition", Texture_Map::POSITION);
		shader.updateUniform("gNormal", Texture_Map::NORMAL);
		shader.updateUniform("gDiffuse", Texture_Map::DIFFUSE);
	}

	void DirectionalLightPass::draw(Renderer& renderer, Shader& shader)
	{
		
		renderer.disable(Renderer::DEPTH_WRITE);
		renderer.enable(Renderer::DEPTH_READ);
		renderer.enable(Renderer::FACE_CULLING);
		
		shader.updateUniform("eyePosition",  renderer.getCamera()
											 .getTransform()
											 ->getPosition());
		shader.updateUniform("screenSize", 
							 renderer.getScreenSize());

		for(DirectionalLight light : m_lights)
		{
			light.updateShader(shader, renderer.getCamera()
												.getViewProjection());

			m_mesh.draw();
		}

		renderer.disable(Renderer::DEPTH_READ);
		renderer.enable(Renderer::FACE_CULLING);
	}	
	
	void DirectionalLightPass::addLight(const DirectionalLight& light)
	{
		m_lights.push_back(light);
	}

	PointLightPass::PointLightPass(const Mesh& mesh)
		: m_mesh(mesh)	
	{}

	void PointLightPass::init(Renderer& renderer, Shader& shader)
	{
		shader.updateUniform("gPosition", Texture_Map::POSITION);
		shader.updateUniform("gNormal", Texture_Map::NORMAL);
		shader.updateUniform("gDiffuse", Texture_Map::DIFFUSE);
	}

	void PointLightPass::draw(Renderer& renderer, Shader& shader)
	{
		renderer.disable(Renderer::DEPTH_WRITE);
		renderer.enable(Renderer::DEPTH_READ);
		renderer.enable(Renderer::FACE_CULLING);
		
		shader.updateUniform("eyePosition",  renderer.getCamera()
											 .getTransform()
											 ->getPosition());
		shader.updateUniform("screenSize", 
							 renderer.getScreenSize());

		for(PointLight light : m_lights)
		{
			light.updateShader(shader, renderer.getCamera()
												.getViewProjection());

			m_mesh.draw();
		}

		renderer.disable(Renderer::DEPTH_READ);
		renderer.enable(Renderer::FACE_CULLING);
	}
	
	void PointLightPass::addLight(const PointLight& light)
	{
		m_lights.push_back(light);
	}


	AmbientPass::AmbientPass(const Mesh& screen, 
							 const Real intensity) 
		: m_screen(screen), m_intensity(intensity)
	{}

	void AmbientPass::init(Renderer& renderer, Shader& shader)
	{
		shader.updateUniform("gDiffuse", Texture_Map::DIFFUSE);
	}

	void AmbientPass::draw(Renderer& renderer, Shader& shader)
	{
		shader.updateUniform("ambient", m_intensity);
		renderer.disable(Renderer::DEPTH_WRITE | Renderer::DEPTH_READ);

		m_screen.draw();
	}

	void AmbientPass::setIntensity(const Real intensity)
	{
		m_intensity = intensity;
	}
/*
	StencilPass::StencilPass(const Mesh& mesh):m_mesh(mesh)
	{}

	void StencilPass::init(Renderer& renderer, Shader& shader)
	{}

	void StencilPass::draw(Renderer& renderer, Shader& shader)
	{
//		glDrawBuffer(GL_NONE);
		renderer.enable(Renderer::DEPTH_READ);
		renderer.disable(Renderer::FACE_CULLING);
		renderer.clear(0, true, false, true);
		glStencilFunc(GL_ALWAYS, 0, 0);
		glStencilMask(0xFF);

		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

		for(std::shared_ptr<Light> light : m_lights)
		{
			light->updateShader(shader, 
						renderer.getCamera().getViewProjection());

			m_mesh.draw();
		}
		glStencilMask(0x00);
//		renderer.enable(Renderer::BLEND);
//		renderer.disable(Renderer::FRAMEBUFFER);
		//renderer.clear(0.0f, true, false, false);
//		renderer.applyDepthBuffer();
	}

	void StencilPass::addLight(const std::shared_ptr<Light> light)
	{
		m_lights.push_back(light);
	}*/
}
