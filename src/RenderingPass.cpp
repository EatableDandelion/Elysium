#include "RenderingPass.h"

namespace Elysium
{

	Renderer::Renderer(const int width, const int height, const int nbBuffers)
			:m_camera(70.0f, (float)width/(float)height, 0.01f, 1000.0f),
			   m_frame(width, height, nbBuffers)
	{
		readDepthBuffer(true);
		writeDepthBuffer(true);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}

	Renderer::Renderer(const Renderer& other) :	m_camera(other.m_camera), 
												m_frame(other.m_frame)
	{}

	Renderer& Renderer::operator=(const Renderer& other)
	{
		return *this;
	}


	Camera Renderer::getCamera() const
	{
		return m_camera;
	}
	
	Camera& Renderer::getCamera()
	{
		return m_camera;
	}

	void Renderer::readFrameBuffer()
	{
		m_frame.read();
	}

	void Renderer::writeFrameBuffer()
	{
		m_frame.write();
	}	

	void Renderer::writeDepthBuffer(const bool value)
	{
		glDepthMask(value);	
	}

	void Renderer::readDepthBuffer(const bool value)
	{
		if(value)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}
	
	void Renderer::clear(const float color)
	{
		glClearColor(color, color, color, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GeometryPass::init(Renderer& renderer, Shader& shader)
	{}

	void GeometryPass::draw(Renderer& renderer, 
			  				Shader& shader, 
			 				const std::vector<Model>& models)
	{
		renderer.writeFrameBuffer();
		renderer.writeDepthBuffer(true);
		renderer.readDepthBuffer(true);
		renderer.clear(0.0f);

		for(Model model : models)
		{
			shader.updateUniforms(model.getTransform()->getTransformMatrix(), 
								  renderer.getCamera());
			model.draw();
		}
	}

	void FinalPass::init(Renderer& renderer, Shader& shader)
	{
		screen = ResourceManager<Mesh, MeshLoader>("../res/meshes/")
						.getResource("plane3.obj");	
		shader.updateUniform("gPosition", Texture_Map::POSITION);
		shader.updateUniform("gNormal", Texture_Map::NORMAL);
		shader.updateUniform("gDiffuse", Texture_Map::DIFFUSE);
	}

	void FinalPass::draw(Renderer& renderer, 
			  			 Shader& shader, 
			  			 const std::vector<Model>& models)
	{
		renderer.readFrameBuffer();
		renderer.clear(1.0f);
		renderer.writeDepthBuffer(false);
		renderer.readDepthBuffer(false);

		screen.draw();
	}
}
