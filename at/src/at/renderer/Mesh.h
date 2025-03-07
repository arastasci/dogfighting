#pragma once
#include "atpch.h"
#include "VertexArray.h"

namespace at
{
	class  Mesh
	{
	public:
		Mesh();
		~Mesh();
		void LoadMesh(std::string& path);
		
	private:
		std::shared_ptr<VertexArray> m_VertexArray;

	};
}