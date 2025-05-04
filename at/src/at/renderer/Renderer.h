#pragma once

#include "VertexArray.h"
#include "Mesh.h"
#include "Shader.h"
#include "Light.h"
#include "at/utils/Constants.h"

namespace at
{
	class Renderer
	{
	public:
		static void Init();
		static void SetViewport(int x, int y, int width, int height);

		static void SetPointLight(const PointLight& light);
		static void DrawElements(std::shared_ptr<VertexArray>& vertexArray, std::vector<Texture>& textures, std::shared_ptr<Shader> shader);

	private:
		PointLight pointLights[Constants::PointLightCount];
		short pointLightIndex{ 0 };
	};
}