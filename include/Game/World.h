#pragma once
#include "Game/ECS.h"

namespace Elysium
{
	class Context;

	class World
	{
		public:
			void init(const std::shared_ptr<GameInterface> game);

			Entity newEntity();

			void removeEntity(const Entity entity);

			void update(const Real dt, std::shared_ptr<Renderer> renderer);

			std::vector<Entity> getEntities() const;

		private:
			std::vector<Entity> m_entities;
			std::shared_ptr<GameInterface> m_context;
	};
}
