#include "Physics/Physics.h"

namespace Physics
{

	GravityGenerator::GravityGenerator(const Real g):g(g)
	{}

	void GravityGenerator::apply(const Real dt, 
								 PhysicsComponent& component,
			   					 Transform transform)
	{
		component.addForce(Vec2(0.0,-g/component.getMassInv()));
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
							   Elysium::World& world,
							   Elysium::Context& context)
	{
		if(dt > 1.0)return;

		for(Elysium::Entity entity : world.getEntities())
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

				component->update(dt, component->getLoads());

				for(Elysium::Entity entity2 : world.getEntities())
				{
					if(entity2->getID() <= entity->getID()) continue;
					if(entity2->hasComponent<PhysicsComponent>())
					{
						std::shared_ptr<PhysicsComponent> component2 =
								entity2->getComponent<PhysicsComponent>();

						std::shared_ptr<Contact> contact =
							m_detector.collide(component->getCollider(),
											   component2->getCollider());

						if(contact != nullptr)
						{
							m_contacts.push(
								std::make_shared<Collision>(component,
									component2, contact->positionBody1,
									contact->positionBody2));
						}
					}	
				}
			}
		}

		while(!m_contacts.empty())
		{
			m_constraintSolver.applyImpulse(dt, m_contacts.top());
			m_contacts.pop();
		}

		for(std::shared_ptr<Joint> joint : m_joints)
		{
			m_constraintSolver.applyImpulse(dt, joint);
		}

		for(Elysium::Entity entity : world.getEntities())
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
