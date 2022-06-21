#pragma once

#include "Game/Game.h"

namespace Elysium
{
	class CameraComponent : public Component
	{
		public:
		private:
	};

	class CameraSystem : public System
	{
		public:
			CameraSystem();

			virtual void update(std::shared_ptr<GameInterface> game);
	};
}
