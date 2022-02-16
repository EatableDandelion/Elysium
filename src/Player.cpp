#include "Player.h"

namespace Elysium
{
	Mover::Mover(const std::shared_ptr<Circe::Transform3> transform):
		m_transform(transform)
	{
		m_forward = std::make_shared<Listener<int>>([this](int state)
				{
					if(state == 1)
						m_transform->translate(Circe::Vec3(0,0,0.1f));
				});
		m_backward = std::make_shared<Listener<int>>([this](int state)
				{
					if(state == 1)
						m_transform->translate(Circe::Vec3(0,0,-0.1f));
				});
		m_left = std::make_shared<Listener<int>>([this](int state)
				{
					if(state == 1)
						m_transform->translate(Circe::Vec3(-0.1f,0,0));
				});
		m_right = std::make_shared<Listener<int>>([this](int state)
				{
					if(state == 1)
						m_transform->translate(Circe::Vec3(0.1f,0,0));
				});	
		m_rotate = 
			std::make_shared<Listener<Circe::Vec2>>([this](Circe::Vec2 dx)
				{
					Circe::Vec3 axis1(0.0f,1.0f,0);
					Circe::Vec3 axis2(1.0f,0.0f,0);

					axis1 = m_transform->toGlobal(axis1, false);
					axis2 = m_transform->toGlobal(axis2, false);

					m_transform->rotate(axis1,-dx(0)*0.005f);
					m_transform->rotate(axis2,-dx(1)*0.005f);
				});	
	}

	SharedListener<int> Mover::getForwardListener() const
	{
		return m_forward;
	}

	SharedListener<int> Mover::getBackwardListener() const
	{
		return m_backward;
	}

	SharedListener<int> Mover::getLeftListener() const
	{
		return m_left;
	}

	SharedListener<int> Mover::getRightListener() const
	{
		return m_right;
	}

	SharedListener<Circe::Vec2> Mover::getRotateListener() const
	{
		return m_rotate;
	}
}
