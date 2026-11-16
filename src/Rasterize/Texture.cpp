#include"Texture.h"
#include"stb_image.h"
#include"stb_image_write.h"
#include"../Log.h"
#include<format>


uint32_t BytesPerPixel(TextureFormat format)
{
	switch (format)
	{
		case TextureFormat::eR8:
			return 1;
		case TextureFormat::eRG8:
			return 2;
		case TextureFormat::eRGB8:
			return 3;
		case TextureFormat::eRGBA8:
			return 4;
		case TextureFormat::eRGB32F:
			return 12;
		case TextureFormat::eR32F:
			return 4;
	}
}

Texture::Texture(const std::string& path)
	:m_Data(nullptr)
{
	static bool firstTime = true;
	if (firstTime)
	{
		stbi_set_flip_vertically_on_load(true);
		firstTime = false;
	}

	int32_t width, height, channel;
	uint8_t* pData;
	if (stbi_is_hdr(path.c_str()))
	{
		pData = (uint8_t*)stbi_loadf(path.c_str(), &width, &height, &channel, 0);
		ASSERT(pData, "Failed to load HDR texture: %s", path.c_str());
		m_Format = TextureFormat::eRGB32F;
	}
	else
	{
		pData = stbi_load(path.c_str(), &width, &height, &channel, 0);
		ASSERT(pData, "Failed to load HDR texture: %s", path.c_str());
		switch (channel)
		{
		case 4:
			m_Format = TextureFormat::eRGBA8;
			break;
		case 3:
			m_Format = TextureFormat::eRGB8;
			break;
		case 2:
			m_Format = TextureFormat::eRG8;
			break;
		case 1:
			m_Format = TextureFormat::eR8;
			break;
		}
	}
	m_Width = width;
	m_Height = height;
	uint32_t size = m_Width * m_Height * BytesPerPixel(m_Format);
	m_Data = (uint8_t*)malloc(size);
	memcpy(m_Data, pData, size);
	stbi_image_free(pData);
}

Texture::Texture(uint32_t width, uint32_t height, TextureFormat format)
	: m_Width(width), m_Height(height), m_Data(nullptr)
{
	m_Format = format;
	uint32_t size = m_Width * m_Height * BytesPerPixel(m_Format);
	m_Data = (uint8_t*)malloc(size);
}

Texture::~Texture()
{
	if (m_Data)
	{
		free(m_Data);
	}
}

glm::vec4 Texture::Sample(glm::vec2 uv)
{
	uv = glm::clamp(uv, glm::vec2(0.0f), glm::vec2(1.0f));
	uv *= glm::vec2(m_Width, m_Height);
	glm::ivec2 pos = uv;
	switch (m_Format)
	{
	case TextureFormat::eRGB8:
	{
		uint32_t index = (pos.y * m_Width + pos.x) * 3;
		return glm::vec4(m_Data[index], m_Data[index + 1], m_Data[index + 2], 255.0f) * (1 / 255.0f);
	}
	case TextureFormat::eRGBA8:
	{
		uint32_t index = (pos.y * m_Width + pos.x) * 4;

		return glm::vec4(m_Data[index] , m_Data[index + 1] , m_Data[index + 2] , m_Data[index + 3] ) * (1 / 255.0f);
	}
	case TextureFormat::eR32F:
	{
		uint32_t index = (pos.y * m_Width + pos.x);
		float* data = (float*)m_Data;
		return glm::vec4(m_Data[index], 0, 0, 1);
	}
	}
}

glm::vec4 Texture::SamplePixel(glm::ivec2 pixel)
{
	if (pixel.x < 0 || pixel.x >= m_Width || pixel.y < 0 || pixel.y >= m_Height)
		return { 0,0,0,1 };
	switch (m_Format)
	{
	case TextureFormat::eRGB8:
	{
		uint32_t index = (pixel.y * m_Width + pixel.x) * 3;
		return glm::vec4{ m_Data[index],m_Data[index + 1],m_Data[index + 2],255.0f } * (1 / 255.0f);
	}
	case TextureFormat::eRGBA8:
	{
		uint32_t index = (pixel.y * m_Width + pixel.x) * 4;
		return glm::vec4{ m_Data[index],m_Data[index + 1],m_Data[index + 2],m_Data[index+3]} * (1 / 255.0f);
	}
	case TextureFormat::eR32F:
	{
		uint32_t index = (pixel.y * m_Width + pixel.x);
		float* data = (float*)m_Data;
		return glm::vec4(data[index], 0, 0, 1);
	}
	}
}

void Texture::Write(glm::ivec2 pixel, glm::vec4 color)
{
	switch (m_Format)
	{
	case TextureFormat::eRGB8:
	{
		uint32_t index = (pixel.y * m_Width + pixel.x) * 3;
		m_Data[index] = color.r * 255.0f;
		m_Data[index + 1] = color.g * 255.0f;
		m_Data[index + 2] = color.b * 255.0f;
		break;
	}
	case TextureFormat::eRGBA8:
	{
		uint32_t index = (pixel.y * m_Width + pixel.x) * 4;
		m_Data[index] = color.r * 255.0f;
		m_Data[index + 1] = color.g * 255.0f;
		m_Data[index + 2] = color.b * 255.0f;
		m_Data[index + 3] = color.a * 255.0f;
		break;
	}
	case TextureFormat::eR32F:
	{
		uint32_t index = (pixel.y * m_Width + pixel.x);
		float* data = (float*)m_Data;
		data[index] = color.r;
	}
	}
}

void Texture::Clear(glm::vec4 color)
{
	switch (m_Format)
	{
	case TextureFormat::eRGB8:
	{
		uint32_t size = m_Width * m_Height * BytesPerPixel(m_Format);
		color *= 255;
		for (int i = 0; i < size; i+=3)
		{
			m_Data[i] = color.r;
			m_Data[i + 1] = color.g;
			m_Data[i + 2] = color.b;
		}
		break;
	}
	case TextureFormat::eRGBA8:
	{
		uint32_t size = m_Width * m_Height * BytesPerPixel(m_Format);
		color *= 255;
		for (int i = 0; i < size; i += 4)
		{
			m_Data[i] = color.r;
			m_Data[i + 1] = color.g;
			m_Data[i + 2] = color.b;
			m_Data[i + 3] = color.a;
		}
		break;
	}
	case TextureFormat::eR32F:
	{
		uint32_t size = m_Width * m_Height;
		float* data = (float*)m_Data;
		for (int i = 0; i < size; i++)
		{
			data[i] = color.r;
		}
		break;
	}
	}
}

void Texture::Save(const std::string& path)
{
	if (m_Format != TextureFormat::eRGB8)
	{
		LOG("Image Save Only for RGB8");
		return;
	}
	static bool firstTime = true;
	if (firstTime)
	{
		firstTime = false;
		stbi_flip_vertically_on_write(true);
	}
	int w = (int)m_Width;
	int h = (int)m_Height;

	int ok = stbi_write_jpg(
		path.c_str(),
		w, h,
		3,                              // eRGB8
		m_Data,
		100
	);

	if (ok)
	{
		LOG(std::format("Saved: {}", path));
	}
	else
	{
		LOG(std::format("Failed to save: {}", path));
	}
}


