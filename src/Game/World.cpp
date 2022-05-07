#include "Game/World.h"

namespace Elysium
{
	void World::init(const std::shared_ptr<GameInterface> context)
	{
		m_context = context;
	}
	
	Entity World::newEntity()
	{
		Entity entity = std::make_shared<EntityData>(m_context);
		m_entities.push_back(entity);
/*		m_tree.insert(entity->getID(), 
					  entity->getTransform()->getPosition(), 
					  entity->getTransform()->getScale());
*/		return entity;
	}

	void World::removeEntity(const Entity entity)
	{
//		m_tree.remove(entity->getID());
//		m_entities.erase(entity);
	}

	void World::update(const Real dt, std::shared_ptr<Renderer> renderer)
	{
		for(Entity entity : m_entities)
		{
			entity->update(entity, renderer, dt);
/*
			Real mass = 0.0;
			Vec velocity;

			if(entity->hasVariable("mass"))
			{
				mass = entity->getVariable("mass")[0];
			}

			if(entity->hasVariable("velocity"))
			{
				velocity = Vec(entity->getVariable("velocity"));
			}

			m_tree.update(entity->getID(), 
						  entity->getTransform()->getPosition(),
						  velocity, mass);
*/		}
//		m_tree.draw();
	}

	std::vector<Entity> World::getEntities() const
	{
		return m_entities;
	}
}
