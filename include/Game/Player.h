#pragma once
#include <Circe/Circe.h>
#include "Game/Event.h"


namespace Elysium
{
	class Mover
	{
		public:
			Mover(const Transform3 transform);

			SharedListener<int> getForwardListener() const;
			SharedListener<int> getBackwardListener() const;
			SharedListener<int> getLeftListener() const;
			SharedListener<int> getRightListener() const;
			SharedListener<Circe::Vec2> getRotateListener() const;

		private:
			Transform3 m_transform;
			SharedListener<int> m_forward;
			SharedListener<int> m_backward;
			SharedListener<int> m_left;
			SharedListener<int> m_right;
			SharedListener<Circe::Vec2> m_rotate;
	};
}
