#include "Rendering/RenderingPass.h"

namespace Elysium
{

	RenderContext::RenderContext(const int width, 
					   const int height, 
					   const int nbBuffers)
		:m_camera(70.0f, (Real)width/(Real)height, 0.01f, 1000.0f),
		 m_frame(width, height, nbBuffers), m_screenSize(width, height)
	{
		enable(RenderContext::DEPTH_READ);
		enable(RenderContext::DEPTH_WRITE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		glLineWidth(3);
	}
	
	Camera& RenderContext::getCamera()
	{
		return m_camera;
	}

	void RenderContext::enable(const RenderBit kw)
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

	void RenderContext::disable(const RenderBit kw)
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

	void RenderContext::clear(const Real color, const bool diffuse,
						 	  const bool depth, const bool stencil)
	{
		glClearColor(color, color, color, 1.0f);
		
		GLbitfield mask = 0;
		if(diffuse) mask |= GL_COLOR_BUFFER_BIT;
		if(depth) mask |= GL_DEPTH_BUFFER_BIT;
		if(stencil) mask |= GL_STENCIL_BUFFER_BIT;

		glClear(mask);
	}

	void RenderContext::applyDepthBuffer()
	{
		m_frame.applyDepthBuffer();
	}
	
	void RenderContext::writeFBO()
	{
		if(m_frame.isOpen()) return;

		disable(RenderContext::BLEND);
		enable(RenderContext::FRAMEBUFFER);
		enable(RenderContext::DEPTH_WRITE | 
			   RenderContext::DEPTH_READ);
		clear(0.0f);
	}

	void RenderContext::readFBO()
	{
		if(!m_frame.isOpen()) return;

		enable(RenderContext::BLEND);
		disable(RenderContext::FRAMEBUFFER);
		clear(0.0f, true, false, false);
		applyDepthBuffer();
	}


	Circe::Vec2 RenderContext::getScreenSize() const
	{
		return m_screenSize;
	}
	

	void GeometryPass::draw(Model& model,		
					  		const Transform transform, 
							RenderContext& renderer,
							Shader& shader)
	{
		Mat mvp = renderer.getCamera().getViewProjection() 
						*(transform->getTransformMatrix());

		model.setVariable("MVP", mvp);

		model.draw(shader);
	}

	void GeometryPass::draw(RenderContext& renderer, Shader& shader)
	{}


	DebugObject::DebugObject():
			m_transform(std::make_shared<Circe::Trans<DIMENSION>>())
	{}

	void DebugPass::draw(RenderContext& renderer, Shader& shader)
	{
		renderer.disable(RenderContext::DEPTH_WRITE | 
						 RenderContext::DEPTH_READ);

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
	
	void DebugPass::drawBox(const Vec& center, const Vec& halfWidth,
							const Vec3& color)
	{
		Vec p1, p2, p3, p4;
	   	p1(0) = center(0)-halfWidth(0);
	   	p1(1) = center(1)-halfWidth(1);
	   	p2(0) = center(0)-halfWidth(0);
	   	p2(1) = center(1)+halfWidth(1);
	   	p3(0) = center(0)+halfWidth(0);
	   	p3(1) = center(1)+halfWidth(1);
	   	p4(0) = center(0)+halfWidth(0);
	   	p4(1) = center(1)-halfWidth(1);

		drawLine(p1, p2, color);
		drawLine(p2, p3, color);
		drawLine(p3, p4, color);
		drawLine(p4, p1, color);
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
