#pragma once
#include "at/math/Math.h"

namespace at
{
	class Camera
	{
	public:
		Camera(glm::vec3 position, quat rotation, float FOV, float aspectRatio, float nearPlane, float farPlane);
		~Camera() = default;
		mat4 GetProjectionMatrix();
		mat4 GetViewMatrix();
		void Update(glm::vec3 position, quat rotation);
	private:
		mat4 m_ProjectionMatrix;
		mat4 m_ViewMatrix;
		float m_FOV;
		float m_AspectRatio;
		float m_NearPlane;
		float m_FarPlane;
	};
}