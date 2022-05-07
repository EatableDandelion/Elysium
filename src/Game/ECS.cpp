#include "Game/ECS.h"

namespace Elysium
{
	void Component::update(Entity& entity, const Real dt)
	{}

	void Component::draw(Entity& entity, std::shared_ptr<Renderer> renderer)
	{}

	EntityID EntityData::allid=0;
	
	EntityData::EntityData(const std::shared_ptr<GameInterface> comm)
		:id(allid++), 
		 m_transform(std::make_shared<Circe::Trans<DIMENSION>>()),
		 m_gameInterface(comm)
	{
		CIRCE_INFO("Initializing new entity "+std::to_string(id));
	}
	
	EntityData::~EntityData()
	{
		CIRCE_INFO("Terminating entity "+std::to_string(id));
		
		std::shared_ptr<GameInterface> game = m_gameInterface.lock();
		if(game)
		{
			for(auto pair : m_components)
			{
				if(m_components.count(pair.first))
				{
					game->onComponentRemoved(id, pair.first);
				}
			}
		}
		m_components.clear();
	}
	
	void Component::setEntityID(const EntityID id)
	{
		m_id = id;
	}

	EntityID Component::getEntityID() const
	{
		return m_id;
	}

	void EntityData::update(Entity& entity, 
							std::shared_ptr<Renderer> renderer, 
							const Real dt)
	{
		for(auto component : m_components)
		{
			component.second->update(entity, dt);
			component.second->draw(entity, renderer);
		}
	}

	std::vector<Real> EntityData::getVariable(const std::string& name)
	{
		return m_register(name);
	}

	bool EntityData::hasVariable(const std::string& name) const
	{
		return m_register.hasVariable(name);
	}

	EntityID EntityData::getID() const
	{
		return id;
	}
			
	Transform EntityData::getTransform()
	{
		return m_transform;
	}
}
