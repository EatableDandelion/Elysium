#include "Camera.h"

namespace Elysium
{
	Camera::Camera(const Circe::Vec3& position, float fov, float AR,
				   float zNear, float zFar): m_transform(position)
	{
		m_perspective = Circe::perspectiveProjection(fov, AR, zNear, zFar);
	}

	Circe::Mat44 Camera::getViewProjection() const
	{
		Circe::Mat44 rotationMat = Circe::rotationMatrix
									(m_transform.getRotation().conjugate());

		Circe::Vec3 position = m_transform.getPosition()*(-1.0f);

		Circe::Mat44 positionMat = Circe::positionMatrix(position);

		return m_perspective * rotationMat * positionMat;
	}

	Circe::Transform3& Camera::getTransform()
	{
		return m_transform;
	}
}
