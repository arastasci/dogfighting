#pragma once

#include "VertexArray.h"
#include "Mesh.h"
#include "Shader.h"
#include "Light.h"
namespace at
{
	class Renderer
	{
	public:
		static void Init();
		static void SetViewport(int x, int y, int width, int height);

		static void SetPointLight(short id, const PointLight& light, std::shared_ptr<Shader> shader);
		static void DrawElements(std::shared_ptr<VertexArray>& vertexArray, std::vector<Texture>& textures, std::shared_ptr<Shader> shader);
	};
}