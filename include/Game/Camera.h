#pragma once
#include <iostream>
#include <memory>
#include "Circe/Circe.h"

namespace Elysium
{
	class Camera
	{
		public:
			Camera(const Camera&) = delete;
			Camera(Camera&&) = delete;
			Camera& operator=(const Camera&) = delete;

			Camera(Real fov, Real AR,
				   Real zNear, Real zFar);

			Mat getViewProjection() const;

			Transform3 getTransform();

		private:
			Mat m_perspective;
			Transform3 m_transform;
	};
}
