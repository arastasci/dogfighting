#pragma once

#include "atpch.h"
#include "at/core/Logger.h"

namespace at
{
	enum class ShaderDataType
	{
		Float = 0,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Bool,
		Int,
		Int2,
		Int3,
		Int4
	};

	uint32_t GetShaderDataTypeSize(ShaderDataType type);
	uint32_t GetOpenGLType(ShaderDataType type);
	


	struct BufferElement
	{
	public:
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		size_t Offset;
		bool Normalized;

		BufferElement() = default;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false);

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Mat3:    return 3; // 3* float3
			case ShaderDataType::Mat4:    return 4; // 4* float4
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Bool:    return 1;
			}

			AT_CORE_ERROR("Unknown ShaderDataType!");
			return 0;
		}
	};


	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(std::initializer_list<BufferElement> elements);

		uint32_t GetStride() const { return m_Stride; }
		const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		void CalculateOffsetsAndStride();
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};


	class VertexBuffer
	{
	public:
		VertexBuffer(float* vertices, uint32_t size);
		VertexBuffer(uint32_t size);

		void Bind();
		void Unbind();

		void SetData(const void* data, uint32_t size);
		void SetLayout(const BufferLayout& layout);
		const BufferLayout& GetLayout();
	private:
		BufferLayout m_Layout;
		uint32_t m_GLID;
		uint32_t m_Size;
	};


	class IndexBuffer
	{
	public:
		IndexBuffer(uint32_t* indices, uint32_t count);

		void Bind() const;
		void Unbind() const;
		uint32_t GetCount() { return m_Count; };

	private:
		uint32_t m_Count;
		uint32_t m_GLID;
	};

}