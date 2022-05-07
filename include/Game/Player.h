#pragma once
#include <Circe/Circe.h>


namespace Elysium
{
	class Mover
	{
		public:
			Mover(const Transform3 transform);

			Circe::Listener<int> getForwardListener() const;
			Circe::Listener<int> getBackwardListener() const;
			Circe::Listener<int> getLeftListener() const;
			Circe::Listener<int> getRightListener() const;
			Circe::Listener<Circe::Vec2> getRotateListener() const;

		private:
			Transform3 m_transform;
			Circe::Listener<int> m_forward;
			Circe::Listener<int> m_backward;
			Circe::Listener<int> m_left;
			Circe::Listener<int> m_right;
			Circe::Listener<Circe::Vec2> m_rotate;
	};
}
