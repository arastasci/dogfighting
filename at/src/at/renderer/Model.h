#pragma once
#include "atpch.h"

#include "at/core/Core.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "Texture2D.h"
#include "ModelNode.h"

namespace at {

	class AT_API Model
	{
	public:
		Model(const std::string& path);
	private:
		friend class ModelLibrary;
		friend class MeshRenderer;

		static auto CreateModel(const std::string& path)
		{
			return std::make_shared<Model>(path);
		}

		void LoadModel(const std::string& path);
		void ProcessNode(ModelNode* parent, aiNode* node, const aiScene* scene);
		Mesh* ProcessMesh(aiMesh* mesh, const mat4& localMatrix, const aiScene* scene);
		std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
		

		std::string m_Directory;
		std::vector<ModelNode*> m_Nodes;
		std::vector<Mesh*> m_Meshes;
		std::vector<Texture> m_Textures;
		std::vector<Texture> m_texturesLoaded;

	};

}