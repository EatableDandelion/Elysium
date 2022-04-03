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
		glLineWidth(3);
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

	void GeometryPass::startDraw(Renderer& renderer)
	{
		renderer.disable(Renderer::BLEND);
		renderer.enable(Renderer::FRAMEBUFFER);
		renderer.enable(Renderer::DEPTH_WRITE | Renderer::DEPTH_READ);
		renderer.clear(0.0f);
	}
	
	void GeometryPass::draw(Model& model,		
					  		const Transform transform, 
							Renderer& renderer,
							Shader& shader)
	{
		if(!wasInit)
		{
			wasInit = true;
			startDraw(renderer);
		}

		Mat mvp = renderer.getCamera().getViewProjection() 
						*(transform->getTransformMatrix());

		model.setUniform("MVP", mvp);

		model.draw(shader);
	}

	void GeometryPass::draw(Renderer& renderer, Shader& shader)
	{
		renderer.enable(Renderer::BLEND);
		renderer.disable(Renderer::FRAMEBUFFER);
		renderer.clear(0.0f, true, false, false);
		renderer.applyDepthBuffer();
		wasInit = false;
	}


	DebugObject::DebugObject():
			m_transform(std::make_shared<Circe::Trans<DIMENSION>>())
	{}

	void DebugPass::init(Renderer& renderer, Shader& shader)
	{}

	void DebugPass::draw(Renderer& renderer, Shader& shader)
	{
		renderer.disable(Renderer::DEPTH_WRITE | Renderer::DEPTH_READ);

		while(!m_objects.empty())
		{
			DebugObject object = m_objects.top();

			Mat mvp = renderer.getCamera().getViewProjection() 
						*(object.m_transform->getTransformMatrix());

			shader.updateUniform("MVP", mvp);
			shader.updateUniform("lineColor", object.m_color);
			object.m_mesh.setRenderingType(MeshType::WIRE_RENDERING);
			object.m_mesh.draw();
			m_objects.pop();
		}
	}
			
	void DebugPass::drawLine(const Vec& p1, const Vec& p2, 
							 const Vec3& color)
	{
		DebugObject line;
		line.m_transform->lineUp(p1,p2);
		line.m_mesh = m_geometries.getResource("line");
		line.m_color = color;
		m_objects.push(line);
	}
	
	void DebugPass::drawVector(const Vec& position, const Vec& vec, 
							   const Real scale, const Vec3& color)
	{
		DebugObject line;
		line.m_transform->lineUp(position, position+vec*scale);
		line.m_mesh = m_geometries.getResource("arrow");
		line.m_color = color;
		m_objects.push(line);
	}

	void DebugPass::drawBox(const Transform transform, 
							const Vec3& color)
	{	
		DebugObject box;
		box.m_transform->translate(transform->getPosition());
		box.m_transform->setRotation(transform->getRotation());
		box.m_transform->setScale(transform->getScale());
		box.m_mesh = m_geometries.getResource("rectangle");
		box.m_color = color;
		m_objects.push(box);
	}
	
	void DebugPass::drawEllipse(const Transform transform, 
								const Vec3& color)
	{	
		drawEllipse(transform->getPosition(), 
					transform->getScale(),
					color,
					transform->getRotation());
	}

	void DebugPass::drawEllipse(const Vec& position, const Vec& scale, 
							    const Vec3& color, const Quaternion& q)
	{	
		DebugObject ellipse;
		ellipse.m_transform->translate(position);
		ellipse.m_transform->setRotation(q);
		ellipse.m_transform->setScale(scale);
		ellipse.m_mesh = m_geometries.getResource("circle");
		ellipse.m_color = color;
		m_objects.push(ellipse);
	}

}
