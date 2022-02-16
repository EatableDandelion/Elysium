#include "Game.h"

/** Game loop from https://gameprogrammingpatterns.com/game-loop.html */
namespace Elysium
{
			
	std::shared_ptr<Input> Game::getInput()
	{
		return m_input;
	}

	void Game::pollInput()
	{
		m_input->poll();
	}

	bool Game::isTerminated() const
	{
		return m_input->isTerminated();
	}
	
	namespace Time
	{
		typedef std::chrono::steady_clock clock;
		typedef std::chrono::duration<float> duration;
		typedef clock::time_point time_point;
	}

	GameLoop::GameLoop(const std::string& name, const int width,
			   		   const int height, const std::shared_ptr<Game> game)
						:m_renderingEngine(name, width, height, 3),
			   			 m_running(false),
						 m_game(game)
	{
		m_game->init(m_renderingEngine);
	}

	void GameLoop::start()
	{
		if(!m_running)
			run();
	}

	void GameLoop::run()
	{
		double SEC_PER_FRAME = 1.0/60.0;
		m_running = true;
		Time::time_point current, previous;
		double lag = 0.0;

		while(!m_game->isTerminated())
		{	

			current = Time::clock::now();

			double elapsed = 
					Time::duration(current - previous).count();
			
			previous = current;

			lag += elapsed;

			std::cout << "\r" << "Execution time: " << elapsed 
					  << " s           " 			<< std::flush;
		
			m_game->pollInput();
				
			while(lag >= SEC_PER_FRAME)
			{
				m_game->update();
				lag -= SEC_PER_FRAME;
			}

			m_renderingEngine.draw();	
		}		
		m_running = false;
	}

}
