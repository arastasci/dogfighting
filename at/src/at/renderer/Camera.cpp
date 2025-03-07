#include "Camera.h"
namespace at
{
	Camera::Camera(glm::vec3 position, glm::quat rotation,float FOV, float aspectRatio, float nearPlane, float farPlane)
		: m_FOV(FOV), m_AspectRatio(aspectRatio), m_NearPlane(m_NearPlane), m_FarPlane(farPlane)
	{
		vec3 pos = position;
		vec3 lookAt = glm::rotate(rotation, Vector3::forward);
		m_ViewMatrix = glm::lookAt(pos, pos + lookAt, Vector3::up);
		m_ProjectionMatrix = glm::perspective(m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane);
	}

	mat4 Camera::GetProjectionMatrix()
	{
		return m_ProjectionMatrix;
	}

	mat4 Camera::GetViewMatrix()
	{
		return m_ViewMatrix;
	}

	void Camera::Update(glm::vec3 position, glm::quat rotation)
	{
		vec3 pos = position;
		vec3 lookAt = glm::rotate(rotation, Vector3::forward);
		m_ViewMatrix = glm::lookAt(pos, pos + lookAt, Vector3::up);
		m_ProjectionMatrix = glm::perspective(m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane);
	}
}
