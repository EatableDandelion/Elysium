#include "Game/ECS.h"

namespace Elysium
{
	void Component::update(Entity& entity, const Real dt)
	{}

	EntityID EntityData::allid=0;
	
	EntityData::EntityData()
		:id(allid++), 
		 m_transform(std::make_shared<Circe::Trans<DIMENSION>>())
	{
		CIRCE_INFO("Initializing new entity "+std::to_string(id));
	}
	
	EntityData::~EntityData()
	{
		CIRCE_INFO("Terminating entity "+std::to_string(id));
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

	EntityID EntityData::getID() const
	{
		return id;
	}
			
	Transform EntityData::getTransform()
	{
		return m_transform;
	}
}
