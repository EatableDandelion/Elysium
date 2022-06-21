#include "Physics/Kepler.h"

namespace Physics
{
	void Orbit::update(const Circe::GravityWell& object,
					   const std::set<Circe::GravityWell>& objects)
	{
		m_object = object;	
		if(objects.size() == 0) return;
		soi = *(objects.begin());
	}

	Vec Orbit::getCircularVelocity() const
	{
		if(m_object.mass + soi.mass == 0) return Vec2(0,0);

		Vec dx = m_object.position - soi.position;
		Real a = std::sqrt(dot(dx,dx));
		Real speed = std::sqrt(Constants::GRAVITY *
							   (m_object.mass + soi.mass) / a);
		
		return Vec2(speed*dx(1)/a, -speed*dx(0)/a);
	}

	void Orbit::draw() const
	{
	}
}
