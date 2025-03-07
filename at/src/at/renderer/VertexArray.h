#pragma once
#include "atpch.h"
#include "Buffers.h"

namespace at
{
	class VertexArray
	{
	public:
		VertexArray();
		void AddVertexBuffer(std::shared_ptr<VertexBuffer>& buffer);
		void SetIndexBuffer(std::shared_ptr<IndexBuffer>& buffer);
		void Bind() const;
		void Unbind() const;
		std::shared_ptr<IndexBuffer> GetIndexBuffer() const;
	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		uint32_t m_GLID;
		uint32_t m_VertexBufferIndex;
	};
}