#include "Game/ECS.h"

namespace Elysium
{
	void Component::update(Entity& entity, 
						   std::shared_ptr<GameInterface> game)
	{}

//	void Component::draw(Entity& entity, std::shared_ptr<Renderer> renderer)
//	{}

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
							std::shared_ptr<GameInterface> game)
	{
		for(auto component : m_components)
		{
			component.second->update(entity, game);
//			component.second->update(entity, dt);
//			component.second->draw(entity, renderer);
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
			
	bool EntityData::hasComponent(const ComponentID componentID) const
	{
		return m_components.count(componentID);
	}

	Transform EntityData::getTransform()
	{
		return m_transform;
	}
			
	Vec EntityData::getPosition() const
	{
		return m_transform->getPosition();
	}

	void System::onComponentAdded(Entity entity, const ComponentID id)
	{
		if(isCompatible(entity))
		{
			m_entities.insert(entity->getID());
		}
	}

	void System::onComponentRemoved(Entity entity, const ComponentID id)
	{
		if(!isCompatible(entity) && hasEntity(entity->getID()))
		{
			m_entities.erase(entity->getID());
		}
	}

	bool System::isCompatible(const Entity& entity) const
	{
		for(ComponentID id : m_key)
			if(!entity->hasComponent(id)) return false;

		return true;
	}
			
	bool System::hasEntity(const EntityID id) const
	{
		return m_entities.find(id) != m_entities.end();
	}
}
