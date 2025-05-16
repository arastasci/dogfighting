#pragma once
#include "at/utils/Math.h"
#include "at/renderer/Renderer.h"
#include "at/ecs/System.h"
#include "at/ecs/CoreComponents/MeshRenderer.h"
#include "at/ecs/CoreComponents/CameraComponent.h"
#include "at/ecs/CoreComponents/Transform.h"
#include "at/renderer/RenderWorld.h"
#include "at/ecs/CoreComponents/PointLightComponent.h"
#include "at/ecs/CoreComponents/DirectionalLightComponent.h"
namespace at
{
	inline float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	class MeshRendererSystem : public System
	{
	public:
		MeshRendererSystem()
		{
			std::vector<std::string> faces
			{
				"right.jpg",
						"left.jpg",
						"top.jpg",
						"bottom.jpg",
						"front.jpg",
						"back.jpg"
			};
			std::string dir = "res/skybox/";
			m_CubemapTexture = loadCubemap(dir, faces);

			glGenVertexArrays(1, &skyboxVAO);
			glGenBuffers(1, &skyboxVBO);
			glBindVertexArray(skyboxVAO);
			glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		}

		void UpdateCameraProperties()
		{
			auto view = GetView<CameraComponent, Transform>();

			if (!m_MainCamera)
			{
				m_MainCamera = { *view.begin(), m_Scene };
			}
			
			Camera c = view.get<CameraComponent>((entt::entity)m_MainCamera).Camera;
			auto& t = view.get<Transform>((entt::entity)m_MainCamera);

			c.Update(t.GetWorldTransform());
			ProjectionMatrix = c.GetProjectionMatrix();
			ViewMatrix = c.GetViewMatrix();
			ViewPosition = t.position;
		}

		void DrawCubemap()
		{
			// draw skybox as last
			glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
			m_CubemapShader.use();
			auto view = glm::mat4(glm::mat3(ViewMatrix)); // remove translation from the view matrix
			m_CubemapShader.setMat4("view", view);
			m_CubemapShader.setMat4("projection", ProjectionMatrix);
			// skybox cube
			glBindVertexArray(skyboxVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTexture);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}

		void RenderEntities()
		{
			auto view = GetView<MeshRenderer, Transform>();
			for(auto& [e,  meshRenderer, transform] : view.each())
			{
				if (!meshRenderer.IsEnabled)
					continue;
				auto shader = meshRenderer.Material.MatShader;
				shader->use();
				meshRenderer.Material.UploadUniforms();
				shader->setMat4("projection", ProjectionMatrix);
				shader->setMat4("view", ViewMatrix);
				shader->setVec3("viewPos", ViewPosition);

				auto model = transform.GetWorldTransform();
				//model = glm::scale(model, transform.scale);
				auto meshes = meshRenderer.GetMeshes();
					

				for (auto* mesh : meshes) {
					model = mesh->GetModelMatrix() * model;
					shader->setMat4("model", model);
					Renderer::DrawElements(mesh->GetVertexArray(), mesh->GetTextures(), meshRenderer.Material.MatShader);
				}

				RenderWorld::Get().Flush();
			}

		}

		void RegisterPointLights()
		{
			auto view = GetView<PointLightComponent, Transform>();
			short i = 0;
			for (auto [e, p, t] : view.each())
			{
				if (i == Constants::PointLightCount)
					break;
				RenderWorld::Get().SetPointLight(p.PointLight, t.position);
			}

			auto dView = GetView<DirectionalLightComponent>();
			auto e = dView.front();
			auto [d] = dView.get(e);
			RenderWorld::Get().SetDirectionalLight(d.light);
		}

	private:
		mat4 ProjectionMatrix;
		mat4 ViewMatrix;
		vec3 ViewPosition;

		Entity m_MainCamera;

		unsigned int m_CubemapTexture;
		Shader m_CubemapShader = Shader("res/shaders/skybox_v.glsl", "res/shaders/skybox_f.glsl");
		unsigned int skyboxVAO, skyboxVBO;

		unsigned int loadCubemap(std::string& dir, std::vector<std::string>& faces)
		{
			unsigned int textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

			int width, height, nrChannels;
			for (unsigned int i = 0; i < faces.size(); i++)
			{
				auto path = dir + faces[i];
				unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
				if (data)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
						0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
					);
					stbi_image_free(data);
				}
				else
				{
					std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
					stbi_image_free(data);
				}
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			return textureID;
		}

	};
}

