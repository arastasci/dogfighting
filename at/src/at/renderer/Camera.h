#pragma once
#include "at/utils/Math.h"

namespace at
{
	class Camera
	{
	public:
		Camera(glm::vec3 position, quat rotation, float FOV, float aspectRatio, float nearPlane, float farPlane);
		Camera(vec3 position, vec3 front, vec3 up, float FOV, float aspectRatio);
		Camera() = default;
		~Camera() = default;
		mat4 GetProjectionMatrix();
		mat4 GetViewMatrix();

		void Update(const mat4& cameraTransform);
	private:
		mat4 m_ProjectionMatrix;
		mat4 m_ViewMatrix;
		float m_FOV;
		float m_AspectRatio;
		float m_NearPlane;
		float m_FarPlane;
	};
}