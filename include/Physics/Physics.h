#pragma once
#include "Game/ECS.h"
#include "Circe/Circe.h"
#include "Physics/PhysicsComponent.h"
#include "Physics/Constraint.h"

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
								std::vector<Elysium::Entity>& entities,
								Elysium::Context& context);
	
			void addConstraint(const std::shared_ptr<Joint>& joint);

		private:
			std::vector<std::shared_ptr<Joint>> m_joints;
			ConstraintSolver m_constraintSolver;
	};
}
