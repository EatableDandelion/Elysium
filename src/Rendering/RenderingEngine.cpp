#include "Rendering/RenderingEngine.h"

namespace Elysium
{
	RenderingNode::RenderingNode() : 
				m_id(std::type_index(typeid(RenderingNode)))
	{}

	RenderingNode::RenderingNode(const Shader& shader) : m_shader(shader),
				m_id(std::type_index(typeid(RenderingNode)))
	{}

	void RenderingNode::init(RenderContext& renderer)
	{
		m_shader.bind();
		m_pass->init(renderer, m_shader);

		if(m_next != nullptr)
			m_next->init(renderer);
	}

	void RenderingNode::draw(RenderContext& renderer)
	{
		m_shader.bind();
		m_pass->draw(renderer, m_shader);

		if(m_next != nullptr)
			m_next->draw(renderer);
	}

	PassID RenderingNode::getID() const
	{
		return m_id;
	}

	Renderer::Renderer(const std::string& name, 
									 const int width,
									 const int height,
									 const int nbBuffers)
					: m_display(width, height, name), 
					  m_renderer(width, height, nbBuffers)
	{}

	void Renderer::init()
	{
		m_wasInit = true;

		if(m_next != nullptr)
			m_next->init(m_renderer);
	}

	void Renderer::draw(Model& model, const Transform transform)
	{
		if(!m_shaderBound)
		{
			m_shaderBound = true;
			m_shader.bind();
		}

		m_pass->draw(model, transform, m_renderer, m_shader);
	}

	void Renderer::draw()
	{
		if(!m_wasInit)
		{
			init();
		}

		RenderingNode::draw(m_renderer);
		m_shaderBound = false;
		swapBuffers();
	}

	Camera& Renderer::getCamera()
	{
		return m_renderer.getCamera();
	}

	void Renderer::swapBuffers()
	{
		m_display.update();
	}
}
