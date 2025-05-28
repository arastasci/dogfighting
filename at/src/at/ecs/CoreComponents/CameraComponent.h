#pragma once
#include "at/core/Core.h"
#include "at/renderer/Camera.h"

namespace at
{
	struct AT_API CameraComponent 
	{
		CameraComponent(vec3 position, vec3 front, vec3 up, float FOV, float aspectRatio) 
			: Camera(position, front, up, FOV, aspectRatio){}
		Camera Camera;

		
	};
}