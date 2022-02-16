#include "Camera.h"

namespace Elysium
{
	Camera::Camera(float fov, float AR,
				   float zNear, float zFar)
		:m_transform(std::make_shared<Circe::Transform3>())
	{
		m_perspective = Circe::perspectiveProjection(fov, AR, zNear, zFar);
	}

	Circe::Mat44 Camera::getViewProjection() const
	{
		Circe::Mat44 rotationMat = Circe::rotationMatrix
								(m_transform->getRotation());
								//(m_transform->getRotation().conjugate());

		Circe::Vec3 position = m_transform->getPosition()*(-1.0f);

		Circe::Mat44 positionMat = Circe::positionMatrix(position);

		return m_perspective * rotationMat * positionMat;
	}

	std::shared_ptr<Circe::Transform3> Camera::getTransform()
	{
		return m_transform;
	}
}
