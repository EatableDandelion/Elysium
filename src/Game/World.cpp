#include "Game/World.h"

namespace Elysium
{
	Entity World::newEntity()
	{
		Entity entity = std::make_shared<EntityData>();
		m_entities.push_back(entity);
		m_tree.insert(entity->getID(), 
					  entity->getTransform()->getPosition(), 
					  entity->getTransform()->getScale());
		return entity;
	}

	void World::removeEntity(const Entity entity)
	{
		m_tree.remove(entity->getID());
//		m_entities.erase(entity);
	}

	void World::update(std::shared_ptr<Renderer> renderer, const Real dt)
	{
	//	for(auto pair : m_entities)
		for(Entity entity : m_entities)
		{
			entity->update(entity, renderer, dt);

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
		}
	}

	std::vector<Entity> World::getEntities() const
	{
		return m_entities;
	}
/*
	void World::update(const EntityID id, 
					   const Vec& position,
					   const Vec& velocity = Vec(), 
					   const Real mass = 0.0)
	{
		//m_tree.update(id, position, velocity, mass);
	}
*/
/*	Entity World::operator()(const EntityID id)
	{
		return m_entities[id];
	}
*/			
		
}
