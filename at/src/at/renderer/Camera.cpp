#include "Camera.h"
namespace at
{
	Camera::Camera(glm::vec3 position, glm::quat rotation,float FOV, float aspectRatio, float nearPlane, float farPlane)
		: m_FOV(FOV), m_AspectRatio(aspectRatio), m_NearPlane(nearPlane), m_FarPlane(farPlane)
	{
		vec3 pos = position;
		m_ViewMatrix = glm::lookAt(pos, pos + Vector3::forward, Vector3::up);
		m_ProjectionMatrix = glm::perspective(m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane);
	}

	Camera::Camera(vec3 position, vec3 front, vec3 up, float FOV, float aspectRatio) 
		: m_FOV(FOV), m_AspectRatio(aspectRatio), m_NearPlane(0.1f), m_FarPlane(800.f)
	{
		vec3 pos = position;
		m_ViewMatrix = glm::lookAt(pos, pos + front, up);
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

	void Camera::Update(const mat4& cameraTransform)
	{
		m_ViewMatrix = glm::inverse(cameraTransform);
		m_ProjectionMatrix = glm::perspectiveLH_ZO(m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane);
	}
}
