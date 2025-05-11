#pragma once
#include "atpch.h"
#include "at/utils/Math.h"
#include "at/ecs/System.h"
#include "at/ecs/CoreComponents/PointLightComponent.h"
#include "at/ecs/CoreComponents/DirectionalLightComponent.h"
#include "at/ecs/CoreComponents/Transform.h"
#include "at/renderer/RenderWorld.h"
#include "at/utils/Constants.h"


namespace at
{
	class PointLightSystem : public System
	{
	public:
		virtual void Update(float dt) override
		{
			auto view = GetView<PointLightComponent, Transform>();
			short i = 0;
			for (auto [e, _,  p, t] : view.each())
			{
				if (i == Constants::PointLightCount)
					break;
				RenderWorld::Get().SetPointLight(p.PointLight, t.position);
			}

			auto dView = GetView<DirectionalLightComponent>();
			auto e = dView.front();
			auto [_, d] = dView.get(e);
			RenderWorld::Get().SetDirectionalLight(d.light);
		}
	private:
	};
}