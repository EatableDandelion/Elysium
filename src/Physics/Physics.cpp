#include "Physics/Physics.h"

namespace Physics
{

	GravityGenerator::GravityGenerator(const Real g):g(g)
	{}

	void GravityGenerator::apply(const Real dt, 
								 PhysicsComponent& component,
			   					 Transform transform)
	{
		component.addForce(Vec2(0.0,-g));
	}
		
	SpringGenerator::SpringGenerator(const Real k, 
									 const Real alpha,
									 const Real l0,
									 const Vec anchor)
						:k(k), alpha(alpha), l0(l0), m_anchor(anchor)
	{}

	void SpringGenerator::apply(const Real dt, 
								 PhysicsComponent& component,
			   					 Transform transform)
	{
		Vec dx = m_anchor-transform->getPosition();
		Real l = norm(dx);
		Vec closingVelocity = component.getVelocity().dot(normalize(dx))
								* component.getVelocity();

		component.addForce(dx*k*(l-l0)/l + closingVelocity*alpha);
	}


	void PhysicsEngine::update(const Real dt, 
							   std::vector<Elysium::Entity>& entities,
							   Elysium::Context& context)
	{
		if(dt > 1.0)return;

		for(Elysium::Entity entity : entities)
		{
			if(entity->hasComponent<PhysicsComponent>())
			{
				std::shared_ptr<PhysicsComponent> component =
							entity->getComponent<PhysicsComponent>();

				for(std::shared_ptr<ForceGenerator> forceGen : 
					component->getForceGenerators())
				{
					forceGen->apply(dt, *component, entity->getTransform());
				}
//					  ->updateLoads(dt);

				component->update(dt, component->getLoads());
//entity->getComponent<PhysicsComponent>()
//					  ->resetLoads();

			}
		}

		for(std::shared_ptr<Joint> joint : m_joints)
		{
			m_constraintSolver.applyImpulse(dt, joint);
		}

		for(Elysium::Entity entity : entities)
		{
			if(entity->hasComponent<PhysicsComponent>())
			{
				entity->getComponent<PhysicsComponent>()
					  ->resetLoads();
			}
		}
	}	

	void PhysicsEngine::addConstraint(const std::shared_ptr<Joint>& joint)
	{
		m_joints.push_back(joint);
	}
}
