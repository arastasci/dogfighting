#pragma once

#include "at/core/Core.h"
#include <at/renderer/Material.h>
#include <at/renderer/Model.h>
#include "at/renderer/MaterialLibrary.h"
#include "at/renderer/ModelLibrary.h"
namespace at
{
	struct AT_API MeshRenderer 
	{
		//MeshRenderer(std::shared_ptr<Model> m, const std::shared_ptr<Material>& mat) : Model(m), Material(*mat) {}
		MeshRenderer(const std::string& modelName, const std::string& materialName ) : Model(ModelLibrary::Get().CreateOrGetModel(modelName)), Material(*MaterialLibrary::Get().CreateOrGetMaterial(materialName)) {}
		~MeshRenderer()
		{

		}
		std::shared_ptr<Model> Model;
		Material Material;

		const std::vector<Mesh*>& GetMeshes()
		{
			return Model->m_Meshes;
		}
		bool IsEnabled = true;

		
	};
}