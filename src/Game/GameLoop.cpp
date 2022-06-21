#include "Game/GameLoop.h"

/** Game loop from https://gameprogrammingpatterns.com/game-loop.html */
namespace Elysium
{
	namespace Time
	{
		typedef std::chrono::steady_clock clock;
		typedef std::chrono::duration<Real> duration;
		typedef clock::time_point time_point;
	}

	GameLoop::GameLoop(const std::shared_ptr<Game> game)
						:m_running(false),
						 m_game(game)
	{}

	void GameLoop::start()
	{
		if(!m_running)
			run();
	}

	void GameLoop::run()
	{
		m_game->init();

		double SEC_PER_FRAME = 1.0/60.0;
		m_running = true;
		Time::time_point current, previous, startTime,
						 sysCurrent, sysPrevious;
		double lag = 0.0;
		previous = Time::clock::now();
		startTime = previous;
		sysPrevious = previous;

		while(!m_game->isTerminated() && totalTime < 100.0)
		{	

			current = Time::clock::now();

			double elapsed = Time::duration(current - previous).count();
			
			totalTime = Time::duration(current - startTime).count();

			previous = current;

			lag += elapsed;

			std::cout << "\r" << "Execution time: " << elapsed 
					  << " s           " 			<< std::flush;
		
			while(lag >= SEC_PER_FRAME)
			{
				sysCurrent = Time::clock::now();
				elapsed = Time::duration(sysCurrent - sysPrevious).count();
				sysPrevious = sysCurrent;

				m_game->updateAll(elapsed, totalTime);

				lag -= SEC_PER_FRAME;
			}

			m_game->draw();

		}		
		m_running = false;
	}

	double GameLoop::totalTime = 0.0;

	double GameLoop::GetTotalTime()
	{
		return totalTime;
	}
}
