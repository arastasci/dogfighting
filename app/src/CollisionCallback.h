#pragma once
#include "at.h"

using namespace at;
class CollisionCallback
{

};

class CollisionSystem : public System
{
public:
	void FixedUpdate() override
	{
		auto view = GetView<CollisionCallback, Rigidbody>();

		for (auto [e, rb] : view.each())
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