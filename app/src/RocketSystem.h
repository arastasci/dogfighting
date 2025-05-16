#pragma once
#include "at.h"

using namespace at;

class RocketBehaviour : public BehaviourComponent
{
public:
	RocketBehaviour(const btVector3& vel) 
		: initialVelocity(vel)
	{}
	btVector3 initialVelocity;
	double accTime = 0.0f;
	const float Force = 80.f;
};

class RocketSystem : public System
{
public:
	void Start() override
	{
		auto view = GetStartedView<RocketBehaviour, Transform, Rigidbody>();
		for (auto [e, rocket, tr, rb] : view.each())
		{
			//rb.GetRigidbody()->setDamping(0, 0);
			rb.GetRigidbody()->setLinearVelocity(rocket.initialVelocity + toBt(rocket.Force * tr.Up()));

		}
	}

	void FixedUpdate() override
	{
		auto view = GetView<RocketBehaviour, Rigidbody>();
		for (auto [e, rocket, rb] : view.each())
		{
			rocket.accTime += Constants::FIXED_TIMESTEP;
			if (rocket.accTime >= 2.0f)
			{
				Entity entity = { e, m_Scene };
				entity.DestroyEntity();
			}
			std::vector<Rigidbody*> colliders;
			rb.GetCollidingObjects(colliders);
			if (colliders.size())
			{
				Entity entity = { e, m_Scene };
				entity.DestroyEntity();
			}
		}
	}


};
