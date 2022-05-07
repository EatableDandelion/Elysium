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

	void PhysicsEngine::onComponentAdded(Elysium::Entity entity,
										 const Elysium::ComponentID id)
	{
		if(Elysium::isComponentID<PhysicsComponent>(id))
		{
			std::shared_ptr<PhysicsComponent> component 
							= entity->getComponent<PhysicsComponent>();

			m_tree.insert(entity->getID(), 
						  component->getPosition(),
						  component->getSize(),
						  component->getVelocity(),
						  1.0/component->getMassInv());
		}	
	}
	
	void PhysicsEngine::onComponentRemoved(Elysium::Entity entity,
										 const Elysium::ComponentID id)
	{
		if(Elysium::isComponentID<PhysicsComponent>(id))
		{
			m_tree.remove(entity->getID());
		}
	}



	void PhysicsEngine::update(const Real dt, 
						   std::shared_ptr<Elysium::GameInterface> context)
	{
		//m_tree.draw();

		for(EntityID entityID : m_tree)
		{
			Entity entity = context->getEntity(entityID);
			std::shared_ptr<PhysicsComponent> component 
							= entity->getComponent<PhysicsComponent>();

			m_tree.update(entity->getID(), 
						  component->getPosition(),
						  component->getSize(),
						  component->getVelocity(),
						  1.0/component->getMassInv());
		

			for(std::shared_ptr<ForceGenerator> 
					forceGen : component->getForceGenerators())
			{
				forceGen->apply(dt, *component, entity->getTransform());
			}

			component->update(dt, component->getLoads());
			
			component->resetLoads();
		}
			
		std::vector<Circe::Intersection> contacts = 
										 m_tree.getInternalContacts();

		for(Circe::Intersection pair : contacts)
		{
			Entity entity1 = context->getEntity(pair.id1);
			Entity entity2 = context->getEntity(pair.id2);

			std::shared_ptr<PhysicsComponent> component1 =
					entity1->getComponent<PhysicsComponent>();
			std::shared_ptr<PhysicsComponent> component2 =
					entity2->getComponent<PhysicsComponent>();

			std::shared_ptr<Contact> contact =
				m_detector.collide(component1->getCollider(),
								   component2->getCollider());

			if(contact != nullptr)
			{
				m_contacts.push(std::make_shared<Collision>
						(component1,component2, 
						 contact->positionBody1,
						 contact->positionBody2));
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
	}	

	void PhysicsEngine::addConstraint(const std::shared_ptr<Joint>& joint)
	{
		m_joints.push_back(joint);
	}
}
