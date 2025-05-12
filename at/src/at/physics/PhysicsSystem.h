#pragma once

#include "at/core/Core.h"
#include "at/physics/PhysicsWorld.h"
#include "at/ecs/System.h"
#include "Rigidbody.h"
#include "at/ecs/CoreComponents/Transform.h"
namespace at
{
	class AT_API PhysicsSystem : public System
	{
	public:
		virtual void Start() override
		{
			auto view = GetStartedView<Rigidbody>();
			for (auto [e, _, rb] : view.each())
			{
				rb.SetIsActive(true);

				rb.AddBodyToWorld(m_Scene->GetPhysicsWorld());
				
			}
		}
		virtual void Render(float dt) override
		{
			auto view = GetView<Rigidbody, Transform>();
			for (auto [e, _, rb, t] : view.each())
			{
				if (rb.IsActive())
				{
					if (rb.GetRigidbody()->getMass())
					{
						t = rb.GetInterpolatedTransform(dt);
						t.scale = toGlm(rb.GetRigidbody()->getCollisionShape()->getLocalScaling());
					}
					else
					{
						t = rb.GetStaticWorldTransform();
					}
				}

			}
		}
	};
}