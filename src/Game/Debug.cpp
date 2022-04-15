#include "Game/Debug.h"

void DebugFunctions::setDebugRenderer(
		const std::shared_ptr<Elysium::DebugPass> debugDrawer)
{
	m_pass = debugDrawer;
}

void DebugFunctions::drawLine(const Vec& p1, const Vec& p2, 
							  const Vec3& color)
{
	m_pass->drawLine(p1, p2, color);
}

void DebugFunctions::drawVector(const Vec& position, const Vec& vec, 
				   		   const Real scale, const Vec3& color)
{
	m_pass->drawVector(position, vec, scale, color);
}

void DebugFunctions::drawEllipse(const Vec& position, const Vec& scale, 
						    const Vec3& color, const Quaternion& q)
{
	m_pass->drawEllipse(position, scale, color, q);
}

void DebugFunctions::drawBox(const Transform transform, const Vec3& color)
{
	m_pass->drawBox(transform, color);
}

void DebugFunctions::drawBox(const Vec& p1, const Vec& p2, 
							  const Vec3& color)
{
	m_pass->drawBox(p1, p2, color);
}
