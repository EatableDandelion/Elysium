#pragma once

#include <typeindex>
#include <Circe/BVH.h>
#include "Game/Constants.h"

namespace Elysium
{
	class Component;
	class EntityData;
	using Entity = std::shared_ptr<EntityData>;
	class Input;
	class Renderer;

	class GameInterface
	{
		public:
			virtual Entity getEntity(const unsigned int id) const = 0;

			virtual std::map<unsigned int, Entity> getEntities() const = 0;

			virtual std::vector<unsigned int> query(const 
			std::shared_ptr<Circe::PrimitiveVolume
							<Constants::BroadCollider>> volume) = 0;

			virtual void onComponentAdded(const unsigned int entityID,
					 	  		  const std::type_index componentID) = 0;

			virtual void onComponentRemoved(const unsigned int entityID,
								  const std::type_index componentID) = 0;

			virtual std::shared_ptr<Input> getInput() = 0;

			virtual std::shared_ptr<Renderer> getRenderer() const = 0;

			virtual Vec3 getCameraPosition() const = 0;

			virtual void setCameraPosition(const Vec3 position) = 0;

			virtual Real getTimeStep() const = 0;

			virtual Real getTotalTime() const = 0;
	};
}
