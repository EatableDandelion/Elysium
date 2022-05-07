#pragma once

#include <typeindex>

namespace Elysium
{
	class Component;
	class EntityData;
	using Entity = std::shared_ptr<EntityData>;

	class GameInterface
	{
		public:
			virtual Entity getEntity(const unsigned int id) const = 0;

			virtual std::map<unsigned int, Entity> getEntities() const = 0;

			virtual void onComponentAdded(const unsigned int entityID,
					 	  		  const std::type_index componentID) = 0;

			virtual void onComponentRemoved(const unsigned int entityID,
									 const std::type_index componentID) = 0;
	};
}
