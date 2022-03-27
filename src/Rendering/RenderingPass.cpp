#include "Rendering/RenderingPass.h"

namespace Elysium
{

	Renderer::Renderer(const int width, 
					   const int height, 
					   const int nbBuffers)
		:m_camera(70.0f, (Real)width/(Real)height, 0.01f, 1000.0f),
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

	void Renderer::clear(const Real color, const bool diffuse,
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

	void GeometryPass::draw(std::vector<Entity>& entities,
							Renderer& renderer, Shader& shader)
	{
		renderer.disable(Renderer::BLEND);
		renderer.enable(Renderer::FRAMEBUFFER);
		renderer.enable(Renderer::DEPTH_WRITE | Renderer::DEPTH_READ);
		renderer.clear(0.0f);

		for(Entity entity : entities)
		{
			if(entity->hasComponent<RenderingComponent>())
			{
				std::shared_ptr<RenderingComponent> model = 
							entity->getComponent<RenderingComponent>();

				Mat mvp = renderer.getCamera().getViewProjection() 
							*(entity->getTransform()->getTransformMatrix());

				model->setUniform("MVP", mvp);

				model->draw(shader);
			}
		}

		renderer.enable(Renderer::BLEND);
		renderer.disable(Renderer::FRAMEBUFFER);
		renderer.clear(0.0f, true, false, false);
		renderer.applyDepthBuffer();
	}
	
	void DebugPass::init(Renderer& renderer, Shader& shader)
	{}

	void DebugPass::draw(std::vector<Entity>& entities,
							Renderer& renderer, Shader& shader)
	{
		renderer.disable(Renderer::DEPTH_WRITE | Renderer::DEPTH_READ);

		while(!m_objects.empty())
		{
			DebugObject object = m_objects.top();

			Mat mvp = renderer.getCamera().getViewProjection() 
						*(object.m_transform->getTransformMatrix());

			shader.updateUniform("MVP", mvp);
			object.m_mesh.setRenderingType(MeshType::WIRE_RENDERING);
			object.m_mesh.draw();
			m_objects.pop();
		}

		/*for(Entity entity : entities)
		{
			if(entity->hasComponent<RenderingComponent>())
			{
				std::shared_ptr<RenderingComponent> model = 
							entity->getComponent<RenderingComponent>();

				Mat mvp = renderer.getCamera().getViewProjection() 
							*(entity->getTransform()->getTransformMatrix());

				model->setUniform("MVP", mvp);

				model->draw(shader);
			}
		}*/
	}
			
	void DebugPass::drawLine(const Vec& p1, const Vec& p2)
	{
		DebugObject line;
		line.m_transform->lineUp(p1,p2);
		m_objects.push(line);
	}
}
