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

			Camera(float fov, float AR,
				   float zNear, float zFar);

			Circe::Mat44 getViewProjection() const;

			std::shared_ptr<Circe::Transform3> getTransform();

		private:
			Circe::Mat44 m_perspective;
			std::shared_ptr<Circe::Transform3> m_transform;
	};
}
