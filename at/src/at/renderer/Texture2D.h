#pragma once

#include "atpch.h"
#include "stb_image.h"

namespace at
{
	enum class ImageFormat
	{
		None = 0,
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};

	struct TextureSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		ImageFormat Format = ImageFormat::RGBA8;
		bool GenerateMips = true;
	};


	struct Texture2D
	{
		Texture2D(const TextureSpecification& specification);
		Texture2D(const std::string& path);
		 ~Texture2D();

		 const TextureSpecification& GetSpecification() const { return m_Specification; }

		 uint32_t GetWidth() const  { return m_Width; }
		 uint32_t GetHeight() const  { return m_Height; }
		 uint32_t GetGLID() const  { return m_GLID; }

		 const std::string& GetPath() const  { return m_Path; }

		 void SetData(void* data, uint32_t size) ;

		 void Bind(uint32_t slot = 0) const ;

		 bool IsLoaded() const  { return m_IsLoaded; }

		 bool operator==(const Texture2D& other) const 
		{
			return m_GLID == other.GetGLID();
		}

	private:
		TextureSpecification m_Specification;

		std::string m_Path;
		bool m_IsLoaded = false;
		uint32_t m_Width, m_Height;
		uint32_t m_GLID;
		GLenum m_InternalFormat, m_DataFormat;
	};
}