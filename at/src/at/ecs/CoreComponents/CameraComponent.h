#pragma once
#include "at/core/Core.h"
#include "at/ecs/Component.h"
#include "at/renderer/Camera.h"
namespace at
{
	struct AT_API CameraComponent : public Component
	{
		Camera Camera;
		bool IsMainCamera = false;
	};
}