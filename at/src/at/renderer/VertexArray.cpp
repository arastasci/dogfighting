#include "VertexArray.h"

#include "at/core/Logger.h"
namespace at
{
	VertexArray::VertexArray()
	{
		glCreateVertexArrays(1, &m_GLID);
	}
	void VertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer>& b)
	{
		glBindVertexArray(m_GLID);
		b->Bind();

		const auto& layout = b->GetLayout();
		for (const auto& element : layout)
		{
			switch (element.Type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribPointer(m_VertexBufferIndex,
					element.GetComponentCount(),
					GetOpenGLType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.Offset);
				m_VertexBufferIndex++;
				break;
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribIPointer(m_VertexBufferIndex,
					element.GetComponentCount(),
					GetOpenGLType(element.Type),
					layout.GetStride(),
					(const void*)element.Offset);
				m_VertexBufferIndex++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribPointer(m_VertexBufferIndex,
						count,
						GetOpenGLType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(element.Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(m_VertexBufferIndex, 1);
					m_VertexBufferIndex++;
				}
				break;
			}
			default:
				(false, "Unknown ShaderDataType!");
			}
		}

		m_VertexBuffers.push_back(b);
	}

	void VertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer>& buffer)
	{
		Bind();
		m_IndexBuffer = buffer;
		m_IndexBuffer->Bind();
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_GLID);
	}
	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	std::shared_ptr<IndexBuffer> VertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}


}