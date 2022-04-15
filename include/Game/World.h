#pragma once
#include <map>
#include <Circe/BVH.h>
#include "Game/ECS.h"
#include "Physics/CollisionDetection.h"

namespace Elysium
{
	class World
	{
		public:
			Entity newEntity();

			void removeEntity(const Entity entity);

			void update(std::shared_ptr<Renderer> renderer, const Real dt);

//			Entity operator()(const EntityID id);

			std::vector<Entity> getEntities() const;

		private:
			//std::map<EntityID, Entity> m_entities;
			std::vector<Entity> m_entities;
			Circe::BVH<DIMENSION, Physics::AABB> m_tree;
	};
}
