#pragma once
#include "at.h"

using namespace at;
class CollisionCallback : public Component
{

};

class CollisionSystem : public System
{
public:
	void FixedUpdate() override
	{
		auto view = GetView<CollisionCallback, Rigidbody>();

		for (auto [e, _, cc, rb] : view.each())
		{
			std::vector<Rigidbody*> colliders;
			 rb.GetCollidingObjects(colliders);
			 if (colliders.size())
			 {
				 Logger::GetClientLogger()->info("Hello, i collided!!!");
		     }
		}
	}
};