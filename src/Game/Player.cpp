#include "Game/Player.h"

namespace Elysium
{
	Mover::Mover(const Transform3 transform):
		m_transform(transform)
	{
		m_forward = std::make_shared<Circe::ListenerObject<int>>
										([this](int state)
				{
					if(state == 1)
					{
						Circe::Vec3 axis1(0.0f,0.0f,0.1f);
						axis1 = m_transform->toGlobal(axis1, false);
						m_transform->translate(axis1);
					}
				});
		m_backward = std::make_shared<Circe::ListenerObject<int>>
										([this](int state)
				{
					if(state == 1)
					{
						Circe::Vec3 axis1(0.0f,0.0f,-0.1f);
						axis1 = m_transform->toGlobal(axis1, false);
						m_transform->translate(axis1);
					}
				});
		m_left = std::make_shared<Circe::ListenerObject<int>>
										([this](int state)
				{
					if(state == 1)
					{
						Circe::Vec3 axis1(-0.1f,0.0f,0.0f);
						axis1 = m_transform->toGlobal(axis1, false);
						m_transform->translate(axis1);
					}
				});
		m_right = std::make_shared<Circe::ListenerObject<int>>
										([this](int state)
				{
						Circe::Vec3 axis1(0.1f,0.0f,0.0f);
						axis1 = m_transform->toGlobal(axis1, false);
						m_transform->translate(axis1);
				});	
		m_rotate = 
			std::make_shared<Circe::ListenerObject<Circe::Vec2>>
										([this](Circe::Vec2 dx)
				{
					Circe::Vec3 axis1(0.0f,1.0f,0);
					Circe::Vec3 axis2(1.0f,0.0f,0);

					axis1 = m_transform->toGlobal(axis1, false);
					axis2 = m_transform->toGlobal(axis2, false);

					m_transform->rotate(axis1*dx(0)*0.005f);
					m_transform->rotate(axis2*dx(1)*0.005f);
				});	
	}

	Circe::Listener<int> Mover::getForwardListener() const
	{
		return m_forward;
	}

	Circe::Listener<int> Mover::getBackwardListener() const
	{
		return m_backward;
	}

	Circe::Listener<int> Mover::getLeftListener() const
	{
		return m_left;
	}

	Circe::Listener<int> Mover::getRightListener() const
	{
		return m_right;
	}

	Circe::Listener<Circe::Vec2> Mover::getRotateListener() const
	{
		return m_rotate;
	}
}
