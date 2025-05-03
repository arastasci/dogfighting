#pragma once
#include "atpch.h"
#include "at/utils/Math.h"
#include "System.h"
#include "at/ecs/CoreComponents/PointLightComponent.h>
#include "at/renderer/Renderer.h"
#include MAX_POINTLIGHT 8

namespace at
{
	class PointLightSystem : public System
	{
	public:
		virtual void Update(float dt) override
		{
			auto view = GetView<PointLightComponent>();
			short i = 0;
			for ([e, p] : view.each())
			{
				if (i == MAX_POINTLIGHT)
					break;
				Renderer::SetPointLight(i, p.PointLight, )
			}
		}
	private:
	};
}