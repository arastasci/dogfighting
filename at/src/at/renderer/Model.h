#pragma once
#include "atpch.h"

#include "at/core/Core.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "Texture2D.h"

namespace at {

	class AT_API Model
	{
	private:
		friend class MeshRenderer;
	public:
		static Model CreateModel(const char* path)
		{
			return Model(path);
		}
	private:
		Model(const char* path);

		void LoadModel(std::string path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

		std::string m_Directory;
		std::vector<Mesh> m_Meshes;
		std::vector<Texture> m_Textures;
		std::vector<Texture> m_texturesLoaded;

	};

}