#include "RenderingPass.h"

namespace Elysium
{

	Renderer::Renderer(const int width, 
					   const int height, 
					   const int nbBuffers)
		:m_camera(70.0f, (float)width/(float)height, 0.01f, 1000.0f),
		 m_frame(width, height, nbBuffers), m_screenSize(width, height)
	{
		enable(Renderer::DEPTH_READ);
		enable(Renderer::DEPTH_WRITE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
				
	}
	
	Camera& Renderer::getCamera()
	{
		return m_camera;
	}

/*	Input& Renderer::getInput()
	{
		return m_input;
	}*/
	
	void Renderer::enable(const RenderBit kw)
	{
		if(BLEND & kw)
			glEnable(GL_BLEND);
	
		if(DEPTH_READ & kw)
			glEnable(GL_DEPTH_TEST);
		
		if(DEPTH_WRITE & kw)	
			glDepthMask(true);

		if(STENCIL_READ & kw)
			glEnable(GL_STENCIL_TEST);
		
		if(FRAMEBUFFER & kw)	
			m_frame.write();

		if(FACE_CULLING & kw)
			glEnable(GL_CULL_FACE);
	}

	void Renderer::disable(const RenderBit kw)
	{
		if(BLEND & kw)
			glDisable(GL_BLEND);
			
		if(DEPTH_READ & kw)
			glDisable(GL_DEPTH_TEST);
		
		if(DEPTH_WRITE & kw)	
			glDepthMask(false);

		if(STENCIL_READ & kw)
			glDisable(GL_STENCIL_TEST);
		
		if(FRAMEBUFFER & kw)	
			m_frame.read();
	
		if(FACE_CULLING & kw)
			glDisable(GL_CULL_FACE);
	}

	void Renderer::clear(const float color, const bool diffuse,
						 const bool depth, const bool stencil)
	{
		glClearColor(color, color, color, 1.0f);
		
		GLbitfield mask = 0;
		if(diffuse) mask |= GL_COLOR_BUFFER_BIT;
		if(depth) mask |= GL_DEPTH_BUFFER_BIT;
		if(stencil) mask |= GL_STENCIL_BUFFER_BIT;

		glClear(mask);
	}

	void Renderer::applyDepthBuffer()
	{
		m_frame.applyDepthBuffer();
	}
	
	Circe::Vec2 Renderer::getScreenSize() const
	{
		return m_screenSize;
	}
	
	
	
	
	void GeometryPass::init(Renderer& renderer, Shader& shader)
	{}

	void GeometryPass::draw(Renderer& renderer, Shader& shader)
	{

		renderer.disable(Renderer::BLEND);
		renderer.enable(Renderer::FRAMEBUFFER);
		renderer.enable(Renderer::DEPTH_WRITE | Renderer::DEPTH_READ);
		renderer.clear(0.0f);

		for(Model model : m_models)
		{
			model.draw(shader, renderer.getCamera().getViewProjection());
		}

		renderer.enable(Renderer::BLEND);
		renderer.disable(Renderer::FRAMEBUFFER);
		renderer.clear(0.0f, true, false, false);
		renderer.applyDepthBuffer();
	}

	void GeometryPass::addModel(const Model& model)
	{
		m_models.push_back(model);
	}

}
