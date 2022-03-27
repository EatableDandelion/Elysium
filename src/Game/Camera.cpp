#include "Game/Camera.h"

namespace Elysium
{
	Camera::Camera(Real fov, Real AR,
				   Real zNear, Real zFar)
		:m_transform(std::make_shared<Circe::Trans<3>>()),m_perspective(4,4)
	{
		m_perspective = Circe::perspectiveProjection(fov, AR, zNear, zFar);
	}

	Mat Camera::getViewProjection() const
	{
		Circe::Quaternion rotation = m_transform->getRotation();
		Mat rotationMat = Circe::rotationMatrix(rotation.conjugate());

		Vec3 position = m_transform->getPosition()*(-1.0f);

		Mat positionMat = Circe::positionMatrix(position);

		return m_perspective * rotationMat * positionMat;
	}

	Transform3 Camera::getTransform()
	{
		return m_transform;
	}
}
