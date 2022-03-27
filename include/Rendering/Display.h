#pragma once
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <GL/glew.h>

namespace Elysium
{
	class Display
	{
		public:
			Display(int width, int height, const std::string& name);

			virtual ~Display();

			void update();

/*			void clear(const float r, const float g, 
					   const float b, const float a);
*/			
			bool isClosed() const;

			int getWidth() const;

			int getHeight() const;

		private:
			SDL_Window* m_window;
			SDL_GLContext m_glContext;
			bool m_isClosed;
			int m_width;
			int m_height;
	};
}
