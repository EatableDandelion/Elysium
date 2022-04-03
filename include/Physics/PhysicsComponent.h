#pragma once
#include "Circe/Circe.h"
#include "Game/ECS.h"
#include "Physics/CollisionDetection.h"

namespace Physics
{
	class ForceGenerator;

	class PhysicsComponent : public Elysium::Component
	{
		public:
			PhysicsComponent(const Real mass, const Vec& dimension,
							 Transform transform);	

			void addForce(const Vec& f);

			void addTorque(const Real t);

			void addForce(const Vec& f, const Vec& pointOfAction);

			Vec3 getLoads() const;

			void resetLoads();
			
			void update(const Real dt, const Vec3& loads);

			void addForceGenerator(std::shared_ptr<ForceGenerator> force);

			std::vector<std::shared_ptr<ForceGenerator>> getForceGenerators();

			void setInertia(const Vec2 dimension);

			Real getMassInv() const;

			Real getInertiaInv() const;

			Vec3 getState() const;

			Vec getVelocity() const;

			Real getAngularVelocity() const;

			Vec getPosition() const;
			
			Real getRotation() const;

			Transform getTransform() const;

		private:
			Vec v;
			Real omega;
			Vec3 m_loads;		//Force and moment;
			Real M_inv;			
			Real I_inv;
			
			Shape m_collider;

			Transform m_transform;
			std::vector<std::shared_ptr<ForceGenerator>> m_forces;
	};
}
