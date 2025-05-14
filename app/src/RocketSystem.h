#pragma once
#include "at.h"

using namespace at;

class RocketBehaviour : public BehaviourComponent
{
public:
	const float Force = 80.f;
};

class RocketSystem : public System
{
public:
	void Start() override
	{
		auto view = GetStartedView<RocketBehaviour, Transform, Rigidbody>();
		for (auto [e, _, rocket, tr, rb] : view.each())
		{
			rb.GetRigidbody()->setDamping(0, 0);
			rb.GetRigidbody()->setLinearVelocity(toBt(rocket.Force * tr.Up()));
		}
	}
};
