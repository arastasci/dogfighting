#include "Mesh.h"

namespace at
{

	Mesh::~Mesh()
	{
	}


	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures)
		: m_vertices(vertices), m_indices(indices), m_textures(textures)
	{
		m_VertexArray = std::make_shared<VertexArray>();
		auto vertexBuffer = std::make_shared<VertexBuffer>(sizeof(Vertex) * m_vertices.size() );
		vertexBuffer->SetData(m_vertices.data(), sizeof(Vertex) * m_vertices.size());
		vertexBuffer->SetLayout(BufferLayout(
			{ 
				BufferElement(ShaderDataType::Float3, "aPos"),
				BufferElement(ShaderDataType::Float3, "aNormal"),
				BufferElement(ShaderDataType::Float2, "aTexCoords"),
			}));

		m_VertexArray->AddVertexBuffer(vertexBuffer);

		auto indexBuffer = std::make_shared<IndexBuffer>(m_indices.data(), m_indices.size());

		m_VertexArray->SetIndexBuffer(indexBuffer);

	}


	std::shared_ptr<VertexArray>& Mesh::GetVertexArray()
	{
		return m_VertexArray;
	}
}

