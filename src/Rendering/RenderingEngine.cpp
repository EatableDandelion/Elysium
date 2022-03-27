#include "Rendering/RenderingEngine.h"

namespace Elysium
{
	RenderingNode::RenderingNode() : 
				m_id(std::type_index(typeid(RenderingNode)))
	{}

	RenderingNode::RenderingNode(const Shader& shader) : m_shader(shader),
				m_id(std::type_index(typeid(RenderingNode)))
	{}

	void RenderingNode::init(Renderer& renderer)
	{
		m_shader.bind();
//		m_pass->init(renderer);
		m_pass->init(renderer, m_shader);

		if(m_next != nullptr)
			m_next->init(renderer);
	}

	void RenderingNode::draw(std::vector<Entity>& entities,
							 Renderer& renderer)
	{
		m_shader.bind();
	//	m_pass->draw(entities, renderer);
		m_pass->draw(entities, renderer, m_shader);

		if(m_next != nullptr)
			m_next->draw(entities, renderer);
	}

	PassID RenderingNode::getID() const
	{
		return m_id;
	}

	RenderingEngine::RenderingEngine(const std::string& name, 
									 const int width,
									 const int height,
									 const int nbBuffers)
					: m_display(width, height, name), 
					  m_renderer(width, height, nbBuffers)
	{}



	void RenderingEngine::init()
	{
		m_wasInit = true;

		if(m_next != nullptr)
			m_next->init(m_renderer);
	}

	void RenderingEngine::draw(std::vector<Entity>& entities)
	{
		if(!m_wasInit)
		{
			init();
		}

		if(m_next != nullptr)
			m_next->draw(entities, m_renderer);

		swapBuffers();
	}

	Camera& RenderingEngine::getCamera()
	{
		return m_renderer.getCamera();
	}

	void RenderingEngine::swapBuffers()
	{
		m_display.update();
	}
}
