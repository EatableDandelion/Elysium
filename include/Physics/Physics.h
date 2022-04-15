#pragma once
#include "Game/ECS.h"
#include "Game/World.h"
#include "Circe/Circe.h"
#include "Physics/PhysicsComponent.h"
#include "Physics/Constraint.h"
#include "Physics/CollisionDetection.h"

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
	class PhysicsEngine : public Elysium::System
	{
		public:
			virtual void update(const Real dt,
								Elysium::World& world,
								Elysium::Context& context);
	
			void addConstraint(const std::shared_ptr<Joint>& joint);

		private:
			std::vector<std::shared_ptr<Joint>> m_joints;
			std::stack<std::shared_ptr<Joint>> m_contacts;
			ConstraintSolver m_constraintSolver;
			CollisionDetector m_detector;
	};
}
