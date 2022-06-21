#pragma once
#include "Game/Constants.h"
#include "Circe/BVH.h"

namespace Physics
{
	class Orbit
	{
		public:
			void update(const Circe::GravityWell& object, 
						const std::set<Circe::GravityWell>& objects);

			Vec getCircularVelocity() const;

			void draw() const;
		
		private:
			Circe::GravityWell soi;
			Circe::GravityWell m_object;
	};
}
