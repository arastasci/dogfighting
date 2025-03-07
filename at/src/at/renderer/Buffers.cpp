#include "Buffers.h"

namespace at
{

	uint32_t GetShaderDataTypeSize(ShaderDataType type)
	{
		switch (type) {
		case ShaderDataType::Float:        return 4;
		case ShaderDataType::Float2:   return 4 * 2;
		case ShaderDataType::Float3:   return 4 * 3;
		case ShaderDataType::Float4:   return 4 * 3;
		case ShaderDataType::Mat3: return 4 * 3 * 3;
		case ShaderDataType::Mat4: return 4 * 4 * 4;
		case ShaderDataType::Bool:         return 1;
		case ShaderDataType::Int:          return 4;
		case ShaderDataType::Int2:     return 4 * 2;
		case ShaderDataType::Int3:     return 4 * 3;
		case ShaderDataType::Int4:     return 4 * 4;
		}

		AT_CORE_CRITICAL("Unknown ShaderDataType!");

		return 0;
	}


	uint32_t GetOpenGLType(ShaderDataType type)
	{
		switch (type) {
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
		}

		AT_CORE_CRITICAL("Unknown ShaderDataType!");

		return 0;
	}


	/////////////////////////////////.
	// BUFFER ELEMENT ////////////////
	/////////////////////////////////

	BufferElement::BufferElement(ShaderDataType type, const std::string& name, bool normalized)
		: Name(name), Type(type), Size(GetShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
	{
	}

	/////////////////////////////////
	// BUFFER LAYOUT ////////////////
	/////////////////////////////////
	BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
		: m_Elements(elements)
	{
		CalculateOffsetsAndStride();
	}

	void BufferLayout::CalculateOffsetsAndStride()
	{
		{
			size_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	}

	//////////////////////////////
	// VERTEX BUFFER//////////////
	//////////////////////////////

	VertexBuffer::VertexBuffer(float* vertices, uint32_t size) : m_Size(size)
	{
		glGenBuffers(GL_ARRAY_BUFFER, &m_GLID);

		glBindBuffer(GL_ARRAY_BUFFER, m_GLID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}
	VertexBuffer::VertexBuffer(uint32_t size) : m_Size(size)
	{
		glGenBuffers(GL_ARRAY_BUFFER, &m_GLID);

		glBindBuffer(GL_ARRAY_BUFFER, m_GLID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	void VertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_GLID);
	}

	void VertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_GLID);
	}

	void VertexBuffer::SetData(const void* data, uint32_t size)
	{
		Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void VertexBuffer::SetLayout(const BufferLayout& layout)
	{
		m_Layout = layout;
	}

	const BufferLayout& VertexBuffer::GetLayout()
	{
		return m_Layout;
	}

	//////////////////////////////
	// INDEX BUFFER //////////////
	//////////////////////////////

	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) : m_Count(count)
	{
		glCreateBuffers(1, &m_GLID);

		// we create it as ARRAY BUFFER, bind it as GL_ELEMENT_ARRAY_BUFFER when using
		// because OpenGL doesnt let us bind index buffers without a VAO
		glBindBuffer(GL_ARRAY_BUFFER, m_GLID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_DYNAMIC_DRAW);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GLID);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	
}