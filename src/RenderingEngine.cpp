#include "RenderingEngine.h"

namespace Elysium
{
	RenderingNode::RenderingNode(const Shader& shader, const PassID id) 
								: m_shader(shader), m_id(id)
	{}

	void RenderingNode::init(Renderer& renderer)
	{
		m_shader.bind();
		m_pass->init(renderer, m_shader);

		if(m_next != nullptr)
			m_next->init(renderer);
	}

	void RenderingNode::draw(Renderer& renderer)
	{
		m_shader.bind();
		m_pass->draw(renderer, m_shader, m_models);

		if(m_next != nullptr)
			m_next->draw(renderer);
	}

	void RenderingNode::addModel(const Model& model)
	{
		m_models.push_back(model);
	}

	PassID RenderingNode::getID() const
	{
		return m_id;
	}

	std::shared_ptr<RenderingNode> RenderingNode::getRenderingNode
													(const PassID id) const
	{
		if(m_next == nullptr) CIRCE_ERROR("Could not find rendering node");
		if(m_next->getID() == id)
		{
			return m_next;
		}
		return m_next->getRenderingNode(id);
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
		if(m_firstNode == nullptr)
			CIRCE_ERROR("Rendering engine needs at least one pass.");

		m_firstNode->init(m_renderer);
	}

	void RenderingEngine::draw()
	{
		if(!m_wasInit)
		{
			init();
		}

		if(m_firstNode == nullptr)
			CIRCE_ERROR("Rendering engine needs at least one pass.");

		m_firstNode->draw(m_renderer);

		swapBuffers();
	}

	std::shared_ptr<RenderingNode> RenderingEngine::operator()(const PassID id) 	const
	{
		if(m_firstNode == nullptr)
			CIRCE_ERROR("Could not find rendering node");

		if(m_firstNode->getID() == id)
		{
			return m_firstNode;
		}

		return m_firstNode->getRenderingNode(id);
	}
			
	Camera& RenderingEngine::getCamera()
	{
		return m_renderer.getCamera();
	}

	void RenderingEngine::swapBuffers()
	{
		m_display.update();
	}

	bool RenderingEngine::isClosed() const
	{
		return m_display.isClosed();
	}
}
