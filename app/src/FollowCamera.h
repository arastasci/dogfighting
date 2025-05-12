#pragma once
#include "at.h"
using namespace at;
class FollowCamera : public Component
{
public:
	FollowCamera(const Transform& t, const vec3 offset) 
		: FollowedTransform(t), Offset(offset)
	{
	}
	const Transform& FollowedTransform;
	vec3 Offset;
};

class FollowCameraSystem : public System
{
public:
	virtual void Update(float dt) override
	{
		auto view = GetView<FollowCamera, Transform>();

		for (auto [e, _, fc, t] : view.each())
		{
			// assume the followed transform is a root entity
			auto& worldTransform = fc.FollowedTransform;
			t.position = worldTransform.position + fc.Offset;                            
		}
	}
};