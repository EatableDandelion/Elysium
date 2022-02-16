#include "RenderingEngine.h"

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
		m_pass->init(renderer, m_shader);

		if(m_next != nullptr)
			m_next->init(renderer);
	}

	void RenderingNode::draw(Renderer& renderer)
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

	void RenderingEngine::draw()
	{
		if(!m_wasInit)
		{
			init();
		}

		if(m_next != nullptr)
			m_next->draw(m_renderer);

		swapBuffers();
	}

	Camera& RenderingEngine::getCamera()
	{
		return m_renderer.getCamera();
	}

	/*Input& RenderingEngine::getInput()
	{
		return m_renderer.getInput();
	}*/

	void RenderingEngine::swapBuffers()
	{
		m_display.update();
	}

/*	bool RenderingEngine::isClosed() const
	{
		return m_terminate;
	}*/
	
	

	/*Emitter<int>& RenderingEngine::getKey(const int key)
	{
		return getInput()(key);
	}

	Emitter<Circe::Vec2>& RenderingEngine::getMouseMotion()
	{
		return getInput().getMouseMotion();
	}*/
}
