#include "Input.h"

namespace Elysium
{
	bool Input::m_instantiated = false;

	Input::Input():m_terminate(false)
	{
		assert(!m_instantiated);
		m_instantiated = true;
	}

	void Input::pressKey(const int key, const int state)
	{
		if(m_emitters.count(key))
			m_emitters[key].broadcast(state);

		if(m_keyState.count(key))
			m_keyState[key] = state;
		else
			m_keyState.insert({key, state});
	}
	
	void Input::moveMouse(const int x, const int y, 
						  const int dx, const int dy)
	{
		m_mousePosition(0) = (float)x;
		m_mousePosition(1) = (float)y;
		m_mouseDx(0) = (float)dx;
		m_mouseDx(1) = (float)dy;
		m_mouseMotion.broadcast(m_mouseDx);
	}
	
	
	Emitter<int>& Input::operator()(const int key)
	{
		return getKeyListener(key);	
	}	
			
	Emitter<int>& Input::getKeyListener(const int key)
	{
		if(!m_emitters.count(key))
		{
			m_emitters.insert({key, Emitter<int>()});
		}

		return m_emitters[key];
	}

	Emitter<Circe::Vec2>& Input::getMouseMotion()
	{
		return m_mouseMotion;
	}

	bool Input::isDown(const int key)
	{
		if(!m_keyState.count(key)) m_keyState.insert({key, 0});
		return m_keyState.at(key) == 0;
	}
	
	Circe::Vec2 Input::getMousePosition() const
	{
		return m_mousePosition;
	}
	
	void Input::poll()
	{
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
}
