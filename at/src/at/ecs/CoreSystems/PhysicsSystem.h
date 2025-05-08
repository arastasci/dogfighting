#pragma once

#include "atpch.h"

#include "at/ecs/CoreComponents/RigidbodyComponent.h"
#include "at/ecs/CoreComponents/Transform.h"
#include "at/ecs/System.h"

namespace at
{
	class PhysicsSystem : public System
	{
	public:
		virtual void Update(float dt) override
		{
			auto view = GetView<RigidbodyComponent, Transform>();

			for (auto [e, rb, t] : view.each())
			{
				rb
			}
		}
	private:
	};
}