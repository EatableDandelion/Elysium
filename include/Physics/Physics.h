#pragma once
#include <Circe/Circe.h>
#include <Circe/BVH.h>
#include "Game/Constants.h"
#include "Physics/PhysicsEngine.h"
#include "Physics/PhysicsComponent.h"
#include "Physics/Constraint.h"
#include "Physics/CollisionDetection.h"
#include <set>

namespace Physics
{
	class ForceGenerator
	{
		public:
			virtual void apply(const Real dt,
							   PhysicsComponent& component,
							   Transform transform) = 0;	
	};

	class GravityGenerator : public ForceGenerator
	{
		public: 
			GravityGenerator(const Real g);

			virtual void apply(const Real dt,
							   PhysicsComponent& component,
							   Transform transform);	

		private:
			Real g;
	};

	class SpringGenerator : public ForceGenerator
	{
		public: 
			SpringGenerator(const Real k, 
							const Real alpha, 
							const Real l0,
							const Vec anchor);

			virtual void apply(const Real dt,
							   PhysicsComponent& component,
							   Transform transform);	
		
		private:
			Real k;
			Real alpha;
			Real l0;
			Vec m_anchor;
	};

	class PhysicsEngine : public IPhysicsEngine
	{
		public:
			virtual void onComponentAdded(Entity entity, 
									  	  const ComponentID id);

			virtual void onComponentRemoved(Entity entity, 
									  	  const ComponentID id);

			virtual void update(std::shared_ptr<GameInterface> context);
	
			void addConstraint(const std::shared_ptr<Joint>& joint);

			virtual std::vector<unsigned int> query(const 
			std::shared_ptr<Circe::PrimitiveVolume
							<Constants::BroadCollider>> volume);

		private:
			std::vector<std::shared_ptr<Joint>> m_joints;
			std::stack<std::shared_ptr<Joint>> m_contacts;
			ConstraintSolver m_constraintSolver;
			CollisionDetector m_detector;
			Circe::BVH<DIMENSION, Circe::AABB> m_tree;
	};
}
