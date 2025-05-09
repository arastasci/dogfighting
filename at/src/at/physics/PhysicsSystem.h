#pragma once

#include "at/physics/PhysicsWorld.h"

#include "at/ecs/System.h"
#include "Rigidbody.h"
#include "at/ecs/CoreComponents/Transform.h"
namespace at
{
	class PhysicsSystem : public System
	{
	public:
		virtual void FixedUpdate() override
		{
			auto view = GetView<Rigidbody, Transform>();
			for (auto [e, rb, t] : view.each())
			{
				if (rb.IsActive())
					t = rb.getWorldTransform();
				else
				{
					rb.SetIsActive(true);
					rb.AddBodyToWorld(m_Scene->GetPhysicsWorld());
				}
			}
		}
	};
}