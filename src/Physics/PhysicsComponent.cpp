#include "Physics/PhysicsComponent.h"

namespace Physics
{
	PhysicsComponent::PhysicsComponent(const Real density, 
									 Transform transform,
									 const std::vector<Vec> collisionPoints)
	: m_transform(transform), omega(0),
	  m_collider(collisionPoints, transform), m_torque(0.0)
	{
		Real mass = density;
		for(int i = 0; i<DIMENSION; i++) mass *= transform->getScale()(i);
		M_inv = 1.0/mass; 
		setInertia(transform->getScale());
	}

	void PhysicsComponent::addForce(const Vec& force)
	{
		m_globalLoads(0) = m_globalLoads(0) + force(0);	
		m_globalLoads(1) = m_globalLoads(1) + force(1);	
	}

	void PhysicsComponent::addTorque(const Real t)
	{
		m_torque += t;
	}

	void PhysicsComponent::addForce(const Vec& force, const Vec& PoA)
	{
		addForce(force);
		addTorque(cross(PoA, force));
//		addTorque(cross(pos-m_transform->getPosition(), f));	
	}

	void PhysicsComponent::addLocalForce(const Vec& f, const Vec& PoA)
	{
		m_localLoads(0) = m_localLoads(0) + f(0);
		m_localLoads(1) = m_localLoads(1) + f(1);
		addTorque(cross(PoA, f));
	}

	void PhysicsComponent::resetLoads()
	{
		m_globalLoads = 0.0;
		m_torque = 0.0;
	}
	
	void PhysicsComponent::update(const Real dt)
	{
		v = v + m_globalLoads * dt * M_inv; 
		omega = omega + m_torque * dt * I_inv;

		m_transform->translate(v*dt);
		m_transform->rotate(omega*dt);
	}
	
	void PhysicsComponent::update(Entity& entity, 
								  std::shared_ptr<GameInterface> game)
	{
//		entity->setVariable("velocity", v);
//		entity->setVariable("mass", 1.0/M_inv);
		m_globalLoads = m_globalLoads 
					  +entity->getTransform()->toGlobal(m_localLoads,false);

		m_localLoads.reset();

		//m_collider.draw();
	}

	void PhysicsComponent::addForceGenerator
								(std::shared_ptr<ForceGenerator> force)
	{
		m_forces.push_back(force);
	}

			
	std::vector<std::shared_ptr<ForceGenerator>> 
							PhysicsComponent::getForceGenerators()
	{
		return m_forces;
	}

	void PhysicsComponent::setInertia(const Vec2 dimension)
	{
		I_inv = 5.0/(dimension(0)*dimension(0)+dimension(1)*dimension(1));	
		I_inv *= M_inv;
	}

	Real PhysicsComponent::getMassInv() const
	{
		return M_inv;
	}

	Real PhysicsComponent::getInertiaInv() const
	{
		return I_inv;
	}

	Vec3 PhysicsComponent::getState() const
	{
		return Vec3(m_transform->getPosition()(0),
					m_transform->getPosition()(0),
					m_transform->getRotation().toEulerAngle()(2));
	}

	Vec PhysicsComponent::getVelocity() const
	{
		return v;
	}
	
	Real PhysicsComponent::getAngularVelocity() const
	{
		return omega;
	}

	Vec PhysicsComponent::getPosition() const
	{
		return m_transform->getPosition();
	}

	Real PhysicsComponent::getRotation() const
	{
		return m_transform->getRotation().toEulerAngle()(2);
	}

	Transform PhysicsComponent::getTransform() const
	{
		return m_transform;
	}

	Vec PhysicsComponent::getSize() const
	{
		return m_transform->getScale();
		//return m_size;
	}

/*	void PhysicsComponent::setSize(const Vec& size)
	{
		m_size = size;
	}*/

	Collider PhysicsComponent::getCollider() const
	{
		return m_collider;
	}
}
