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
			for (auto [e, rb] : view.each())
			{
				rb.SetIsActive(true);

				rb.AddBodyToWorld(m_Scene->GetPhysicsWorld(), {e, m_Scene});
				
			}
		}
		void InterpolateTransforms(float dt) 
		{
			auto view = GetView<Rigidbody, Transform>();
			for (auto [e, rb, t] : view.each())
			{
				if (rb.IsActive())
				{
					if (rb.GetRigidbody()->getMass())
					{
						if (m_Interpolates)
							t = rb.GetInterpolatedTransform(dt);
						else
							t = rb.GetWorldTransform();
						t.scale = Math::toGlm(rb.GetRigidbody()->getCollisionShape()->getLocalScaling());
					}
					else
					{
						t = rb.GetStaticWorldTransform();
					}
				}

			}
		}

	private:
		const bool m_Interpolates = false;
	};
}