#pragma once
#include "atpch.h"
#include "VertexArray.h"
#include "at/utils/Math.h"
#include "Texture2D.h"
namespace at
{
	struct Vertex
	{
		vec3 Position;
		vec3 Normal;
		vec2 TexCoords;
	};

	struct Texture {
		unsigned int id;
		std::string type;
		std::string path;
	};

	class AT_API Mesh
	{
	public:
		~Mesh();

		Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);
		std::vector<Texture>& GetTextures() { return m_textures; }
		std::shared_ptr<VertexArray>& GetVertexArray();
	
	private:
		std::shared_ptr<VertexArray> m_VertexArray;
		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indices;
		std::vector<Texture> m_textures;
	};
}