#pragma once
#include <iostream>
#include "Circe/Circe.h"

namespace Elysium
{
	class Camera
	{
		public:
			Camera(float fov, float AR,
				   float zNear, float zFar);

			Circe::Mat44 getViewProjection() const;

			Circe::Transform3& getTransform();

		private:
			Circe::Mat44 m_perspective;
			Circe::Transform3 m_transform;
	};
}
