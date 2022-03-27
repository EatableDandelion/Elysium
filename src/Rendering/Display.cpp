#include "Rendering/Display.h"

namespace Elysium
{
	Display::Display(int width, int height, const std::string& name):
					 m_width(width), m_height(height)
	{
		SDL_Init(SDL_INIT_EVERYTHING);

		int colorSize = 8;
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, colorSize);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, colorSize);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, colorSize);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, colorSize);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, colorSize*4);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		m_window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED,
									SDL_WINDOWPOS_CENTERED,
									width, height, SDL_WINDOW_OPENGL);

		m_glContext = SDL_GL_CreateContext(m_window);

		GLenum status = glewInit();

		if(status != GLEW_OK)
		{
			std::cerr << "GLEW failed to initialize" << std::endl;
		}
		std::cout << "Window created" << std::endl;
		m_isClosed = false;

		std::cout << glGetString(GL_VERSION) << std::endl;
	}

	Display::~Display()
	{
		SDL_GL_DeleteContext(m_glContext);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		std::cout << "Window destroyed" << std::endl;
	}
			
	void Display::update()
	{
		SDL_GL_SwapWindow(m_window);
/*
		SDL_Event e;

		while(SDL_PollEvent(&e))
		{
			switch(e.type)
			{
				case SDL_QUIT:
					m_isClosed = true;
					break;

				case SDL_MOUSEMOTION:
					break;

				case SDL_MOUSEBUTTONDOWN:
					std::cout << e.button.button << std::endl;
					break;

				case SDL_KEYDOWN:
					std::cout << e.key.keysym.sym << std::endl;
					break;
			}
		}
*/	}

	bool Display::isClosed() const
	{
		return m_isClosed;
	}

	int Display::getWidth() const
	{
		return m_width;
	}
			
	int Display::getHeight() const
	{
		return m_height;
	}
}

