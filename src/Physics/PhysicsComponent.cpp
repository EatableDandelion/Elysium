#include "Physics/PhysicsComponent.h"

namespace Physics
{
	PhysicsComponent::PhysicsComponent(const Real mass, 
									   const Vec& dimension,
									   Transform transform)
	: M_inv(1.0/mass), m_transform(transform), omega(0)
	{
		setInertia(dimension);
	}

	void PhysicsComponent::addForce(const Vec& force)
	{
		m_loads(0) = m_loads(0) + force(0);	
		m_loads(1) = m_loads(1) + force(1);	
	}

	void PhysicsComponent::addTorque(const Real t)
	{
		m_loads(2) = m_loads(2) + t;
	}

	void PhysicsComponent::addForce(const Vec& force, const Vec& PoA)
	{
		addForce(force);
		addTorque(cross(PoA, force));
//		addTorque(cross(pos-m_transform->getPosition(), f));	
	}

	Vec3 PhysicsComponent::getLoads() const
	{
		return m_loads;
	}

	void PhysicsComponent::resetLoads()
	{
		m_loads = 0.0;
	}
	
/*	void PhysicsComponent::updateLoads(const Real dt)
	{
		for(std::shared_ptr<ForceGenerator> force : m_forces)
			force->apply(dt, *this, m_transform);
	}
*/	
	void PhysicsComponent::update(const Real dt, const Vec3& loads)
	{
		for(int i = 0; i<2; i++)
			v(i) = v(i) + loads(i) * dt * M_inv; 

		omega = omega + loads(2) * dt * I_inv;
		m_transform->translate(v*dt);
		m_transform->rotate(omega*dt);
	}
	

	void PhysicsComponent::addForceGenerator
								(std::shared_ptr<ForceGenerator> force)
	{
		m_forces.push_back(force);
	}

			
	std::vector<std::shared_ptr<ForceGenerator>> PhysicsComponent::getForceGenerators()
	{
		return m_forces;
	}

	void PhysicsComponent::setInertia(const Vec2 dimension)
	{
		I_inv = 5.0/(dimension(0)*dimension(0)+dimension(1)*dimension(1));	
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
}
