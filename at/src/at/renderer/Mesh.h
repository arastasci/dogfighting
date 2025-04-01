#pragma once
#include "atpch.h"
#include "VertexArray.h"

namespace at
{
	class AT_API Mesh
	{
	public:
		Mesh();
		~Mesh();
		void LoadMesh(std::string& path);
		static std::unordered_map<std::string, std::shared_ptr<Mesh>> MeshMap;
		std::shared_ptr<VertexArray>& GetVertexArray();
	private:
		std::shared_ptr<VertexArray> m_VertexArray;

	};
}