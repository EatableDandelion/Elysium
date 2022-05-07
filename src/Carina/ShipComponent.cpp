#include "Carina/ShipComponent.h"

namespace Carina
{
	void ThrusterSystem::onComponentAdded(Elysium::Entity entity,
										 const Elysium::ComponentID id)
	{
		if(Elysium::isComponentID<ThrusterComponent>(id))
		{
			m_entities.insert(entity.getID());	
		}	
	}
	
	void ThrusterSystem::onComponentRemoved(Elysium::Entity entity,
										 const Elysium::ComponentID id)
	{
		if(Elysium::isComponentID<ThrusterComponent>(id))
		{
			m_entities.erase(m_entity.getID());	
		}
	}
}
