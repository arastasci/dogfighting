#pragma once
#include "atpch.h"
namespace at
{
	template<typename T, typename U>
	using Pair = std::pair<T, U>;

	template<typename T>
	using SharedPtr = std::shared_ptr<T>;

	template<typename T>
	using UniquePtr = std::unique_ptr<T>;

	template<typename T>
	using WeakPtr = std::weak_ptr<T>;
}