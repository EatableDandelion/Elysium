#pragma once
#include <chrono>
#include <ratio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include "Rendering/RenderingEngine.h"
#include "Game/Input.h"
#include "Game/Game.h"
#include "Game/World.h"

namespace Elysium
{
	class GameLoop
	{
		public:
			GameLoop(const std::shared_ptr<Game> game);

			void start();

			static double GetTotalTime();

		private:
			Context m_context;
			World m_world;
			std::shared_ptr<Game> m_game;
			bool m_running;
			static double totalTime;

			void run();
	};
}
