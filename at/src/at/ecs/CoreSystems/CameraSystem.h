#pragma once

#include "at/ecs/System.h"
#include "at/ecs/CoreComponents/CoreComponents.h"
#include "at/ecs/Entity.h"
#include "at/core/Input.h"

namespace at
{
	class MeshRendererSystem;

	class CameraSystem : public System
	{
	public:
		virtual void Update(float dt) override
		{
			
			auto view = GetView<CameraComponent, Transform>();

			if (!m_MainCamera)
			{
				m_MainCamera = { *view.begin(), m_Scene };
			}
			
			Camera c = view.get<CameraComponent>((entt::entity)m_MainCamera).Camera;
			auto& t = view.get<Transform>((entt::entity)m_MainCamera);
			
			

			c.Update(t.position, quat(0, 1, 0, 0));
			ProjectionMatrix = c.GetProjectionMatrix();
			ViewMatrix= c.GetViewMatrix();
		}

		

	private:

		friend class MeshRendererSystem;
		inline static mat4 ProjectionMatrix;
		inline static mat4 ViewMatrix;

		Entity m_MainCamera;

		
	};
}