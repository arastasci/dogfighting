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

	class ModelNode;
	class AT_API Mesh
	{
	public:
		~Mesh();

		Mesh(mat4 transformMatrix, std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);
		std::vector<Texture>& GetTextures() { return m_textures; }
		std::shared_ptr<VertexArray>& GetVertexArray();
		const std::vector<Vertex>& GetVertices() const;
		const std::vector<uint32_t>& GetIndices() const { return m_indices; }

		const mat4& GetModelMatrix() const;
	private:
		std::shared_ptr<VertexArray> m_VertexArray;
		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indices;
		std::vector<Texture> m_textures;
		Mesh* m_ParentMesh;
		mat4 m_ModelMatrix;
	};
}