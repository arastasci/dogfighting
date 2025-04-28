#pragma once

#include "at/renderer/Renderer.h"
#include "at/ecs/System.h"
#include "CameraSystem.h"
#include "at/ecs/CoreComponents/MeshRenderer.h"
namespace at
{
	class MeshRendererSystem : public System
	{
	public:
		virtual void Update(float dt) override
		{
			
			GetView<MeshRenderer, Transform>().each([](MeshRenderer& meshRenderer, auto& transform)
				{
					auto shader = meshRenderer.Material.Shader;
					shader->use();
					shader->setMat4("projection", CameraSystem::ProjectionMatrix);
					shader->setMat4("view", CameraSystem::ViewMatrix);

					auto model = glm::mat4(1.0f);
					model = glm::translate(model, transform.position);
					shader->setMat4("model", model);

					auto meshes = meshRenderer.GetMeshes();
					for (auto& mesh : meshes)
					{
						Renderer::DrawElements(mesh.GetVertexArray(), mesh.GetTextures(), meshRenderer.Material.Shader);
					}
				});

		}
	};
}