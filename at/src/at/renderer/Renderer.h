#pragma once

#include "VertexArray.h"
#include "Shader.h"
namespace at
{
	class Renderer
	{
	public:
		static void Init();
		static void SetViewport(int x, int y, int width, int height);
		static void DrawElements(std::shared_ptr<VertexArray>& vertexArray, std::shared_ptr<Shader> shader);
	};
}