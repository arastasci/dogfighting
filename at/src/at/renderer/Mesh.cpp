#include "Mesh.h"

namespace at
{
	Mesh::Mesh()
	{
		std::string x = "placeholder";
		LoadMesh(x);
	}

	Mesh::~Mesh()
	{
	}

	const float vertices[]
		=
	{
		-0.5f, -0.5f, 0.f,
		0.5f, -0.5f, 0.f,
		0.f, 0.5f, 0.f,
	};

	uint32_t indices[] =
	{
		0, 1, 2
	};


	void Mesh::LoadMesh(std::string& path)
	{
		// TODO: assimp

		m_VertexArray = std::make_shared<VertexArray>();
		auto vertexBuffer = std::make_shared<VertexBuffer>(sizeof(vertices));
		vertexBuffer->SetData(vertices, sizeof(vertices));
		vertexBuffer->SetLayout(BufferLayout({ BufferElement(ShaderDataType::Float3, "aPos") }));

		m_VertexArray->AddVertexBuffer(vertexBuffer);

		auto indexBuffer = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));

		m_VertexArray->SetIndexBuffer(indexBuffer);

		/*	VertexBuffer b(sizeof(vertices));
			b.SetData(vertices, sizeof(vertices));

			BufferLayout l({ BufferElement(ShaderDataType::Float3, "aPos")});
			b.SetLayout(l);

			VertexArray a;

			auto e = std::make_shared<IndexBuffer>(indices, 3);

			auto bPtr = std::make_shared<VertexBuffer>(std::move(b));
			a.AddVertexBuffer(bPtr);

			auto aPtr = std::make_shared<VertexArray>(std::move(a));
			aPtr->SetIndexBuffer(e);*/


	}

	std::shared_ptr<VertexArray>& Mesh::GetVertexArray()
	{
		return m_VertexArray;
	}
}

