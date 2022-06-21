#pragma once
#include <Circe/BVH.h>
#include <Circe/BroadColliders.h>
#include "Game/Constants.h"
#include "Game/ECS.h"

namespace Physics
{
	class IPhysicsEngine : public Elysium::System
	{
		public:
			virtual std::vector<unsigned int> query(const 
				std::shared_ptr<Circe::PrimitiveVolume
								<Constants::BroadCollider>> volume)=0;
	};
}
