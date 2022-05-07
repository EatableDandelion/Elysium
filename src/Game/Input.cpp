#include "Game/Input.h"

namespace Elysium
{
	bool Input::m_instantiated = false;

	Input::Input():m_terminate(false)
	{
		assert(!m_instantiated);
		m_instantiated = true;
	}

	void Input::pressKey(const Key key, const int state)
	{
		if(m_emitters.count(key))
			m_emitters[key].broadcast(state);

		if(state > 0)
			m_activeKeys.push_back(key);
	}

	void Input::pressKey(const std::string& name, const int state)
	{
		pressKey(getKey(name), state);
	}

	void Input::moveMouse(const int x, const int y, 
						  const int dx, const int dy)
	{
		m_mousePosition(0) = (Real)x;
		m_mousePosition(1) = (Real)y;
		m_mouseDx(0) = (Real)dx;
		m_mouseDx(1) = (Real)dy;
		m_mouseMotion.broadcast(m_mouseDx);
	}
	
	
	Circe::Emitter<int>& Input::operator()(const Key key)
	{
		return getKeyListener(key);	
	}	
			
	Circe::Emitter<int>& Input::getKeyListener(const Key key)
	{
		if(!m_emitters.count(key))
		{
			m_emitters.insert({key, Circe::Emitter<int>()});
		}

		return m_emitters[key];
	}

	Circe::Emitter<Circe::Vec2>& Input::getMouseMotion()
	{
		return m_mouseMotion;
	}

	bool Input::isDown(const Key key)
	{
		return !(std::find(m_activeKeys.begin(), m_activeKeys.end(), key)
			               == m_activeKeys.end());
	}

	bool Input::isDown(const std::string& virtualKey)
	{
		return isDown(getKey(virtualKey));
	}

	bool Input::isActive(const std::string& action)
	{
		unsigned int actionID = CIRCE_STRING_ID(action);

		if(!m_actions.count(actionID)) 
		{
			CIRCE_ERROR("Action key "+action+" was never defined.");
			return 0;
		}
		return isDown(m_actions.at(actionID));
	}

	Circe::Vec2 Input::getMousePosition() const
	{
		return m_mousePosition;
	}

	void Input::clear()
	{
		m_activeKeys.clear();
	}

	void Input::poll()
	{
		clear();

		SDL_Event e;

		while(SDL_PollEvent(&e))
		{
			switch(e.type)
			{
				case SDL_QUIT:
					m_terminate = true;
					break;

				case SDL_MOUSEMOTION:
					moveMouse(e.motion.x, e.motion.y,
									  e.motion.xrel, e.motion.yrel);
					break;

				case SDL_MOUSEBUTTONDOWN:
					if(e.button.button == SDL_BUTTON_LEFT)
						pressKey(Input::KEY_MOUSELEFT, 1);
					else if(e.button.button == SDL_BUTTON_RIGHT)
						pressKey(Input::KEY_MOUSERIGHT, 1);
					break;

				case SDL_KEYDOWN:
					pressKey(e.key.keysym.sym, 1);
					break;
			}
		}

	}

	bool Input::isTerminated() const
	{
		return m_terminate;
	}

	void Input::addActionToKey(const Key key, const std::string& action)
	{
		unsigned int actionID = CIRCE_STRING_ID(action);
		if(m_actions.count(actionID))
			m_actions[actionID] = key;
		else
			m_actions.insert(std::pair<unsigned int, Key>(actionID, key));
	}

	Key Input::getKey(const std::string& name) const
	{
		return CIRCE_STRING_ID(name);
	}
}
