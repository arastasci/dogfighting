#pragma once
#include "at/core/Core.h"

#include "entt/entt.hpp"
#include <at/ecs/Scene.h>
#include "ISystem.h"
#include "at/utils/Constants.h"
#include "at/ecs/CoreComponents/CreatedTag.h"
#include "at/ecs/CoreComponents/ActiveTag.h"
#include "at/ecs/CoreComponents/ToBeDestroyedTag.h"
namespace at
{
	class AT_API System : public ISystem
	{
	public:

		virtual void Start() {}
		virtual void Update(float dt){}
		virtual void FixedUpdate(){}
		virtual void OnDestroy() {}
	protected:

		/// <summary>
		/// Use in the Start function.
		/// </summary>
		/// <typeparam name="...T"></typeparam>
		/// <returns></returns>
		template<typename... T>
		auto  GetStartedView()
		{
			return Scene::ActiveScene()->GetAllEntitiesWith<CreatedTag, T...>();
		}

		/// <summary>
		/// Use in the OnDestroy function.
		/// </summary>
		/// <typeparam name="...T"></typeparam>
		/// <returns></returns>
		template<typename... T>
		auto  GetDestroyedView()
		{
			return Scene::ActiveScene()->GetAllEntitiesWith<ToBeDestroyedTag, T...>();
		}

		/// <summary>
		/// Gets the components of an active entity (tagged with the ActiveTag).
		/// </summary>
		/// <typeparam name="...T"></typeparam>
		/// <returns></returns>
		template<typename... T>
		auto  GetView() 
		{
			return Scene::ActiveScene()->GetAllEntitiesWith<ActiveTag, T...>();
		}

		template<typename... T>
		auto  GetGroup()
		{
			return Scene::ActiveScene()->GetAllComponents<T...>();
		}

	};
}