#include "Texture2D.h"
#include "stb_image.h"

namespace at
{
	namespace Utils {

		static GLenum ATImageFormatToGLDataFormat(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::RGB8:  return GL_RGB;
			case ImageFormat::RGBA8: return GL_RGBA;
			}

			return 0;
		}

		static GLenum ATImageFormatToGLInternalFormat(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::RGB8:  return GL_RGB8;
			case ImageFormat::RGBA8: return GL_RGBA8;
			}

			return 0;
		}

	}

	Texture2D::Texture2D(const TextureSpecification& specification)
		: m_Specification(specification), m_Width(m_Specification.Width), m_Height(m_Specification.Height)
	{

		m_InternalFormat = Utils::ATImageFormatToGLInternalFormat(m_Specification.Format);
		m_DataFormat = Utils::ATImageFormatToGLDataFormat(m_Specification.Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_GLID);
		glTextureStorage2D(m_GLID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_GLID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_GLID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_GLID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_GLID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	Texture2D::Texture2D(const std::string& path) : m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}

		if (data)
		{
			m_IsLoaded = true;

			m_Width = width;
			m_Height = height;

			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}

			m_InternalFormat = internalFormat;
			m_DataFormat = dataFormat;


			glCreateTextures(GL_TEXTURE_2D, 1, &m_GLID);
			glTextureStorage2D(m_GLID, 1, internalFormat, m_Width, m_Height);

			glTextureParameteri(m_GLID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_GLID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureParameteri(m_GLID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_GLID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(m_GLID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}
	}
	void Texture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		glTextureSubImage2D(m_GLID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void Texture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(GL_TEXTURE0 + slot, m_GLID);
	}
}
