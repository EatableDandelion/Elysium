#pragma once
#include "Circe/Circe.h"
#include "Game/Game.h"
#include "Physics/CollisionDetection.h"

namespace Physics
{
	class ForceGenerator;

	class PhysicsComponent : public Elysium::Component
	{
		public:
			PhysicsComponent(const Real density,
							 Transform transform,
							 const std::vector<Vec> collisionPoints);	

			void addForce(const Vec& f);

			void addTorque(const Real t);

			void addForce(const Vec& f, const Vec& pointOfAction);

			void addLocalForce(const Vec& f, const Vec& pointOfAction);

			Vec3 getLoads() const;

			void resetLoads();
			
			void update(const Real dt);

			virtual void update(Entity& entity, 
								std::shared_ptr<GameInterface> game);

			void addForceGenerator(std::shared_ptr<ForceGenerator> force);

			std::vector<std::shared_ptr<ForceGenerator>> 
													getForceGenerators();

			void setInertia(const Vec2 dimension);

			Real getMassInv() const;

			Real getInertiaInv() const;

			Vec3 getState() const;

			Vec getVelocity() const;

			Real getAngularVelocity() const;

			Vec getPosition() const;
			
			Real getRotation() const;

			Transform getTransform() const;

			Vec getSize() const;

//			void setSize(const Vec& size);

			Collider getCollider() const;

		private:
			Vec v;
			Real omega;
			Vec m_globalLoads;		//Force and moment;
			Vec m_localLoads;
			Real m_torque;
			Real M_inv;			
			Real I_inv;
//			Vec m_size;

			Collider m_collider;
			Transform m_transform;
			std::vector<std::shared_ptr<ForceGenerator>> m_forces;
	};
}
