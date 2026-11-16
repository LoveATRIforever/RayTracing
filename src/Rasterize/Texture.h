#pragma once
#include"glm/glm.hpp"
#include<string>
#include<cstdint>
#include<memory>

enum class TextureFormat
{
	eR8,
	eRG8,
	eRGB8,
	eRGBA8,
	eRGB32F,
	eR32F
};


uint32_t BytesPerPixel(TextureFormat format);

class Texture
{
private:
	uint32_t m_Width;
	uint32_t m_Height;
	TextureFormat m_Format;
	uint8_t* m_Data;
public:
	Texture(const std::string& path);
	Texture(uint32_t width, uint32_t height, TextureFormat format);
	~Texture();
	Texture(const Texture& other) = delete;
	Texture& operator=(const Texture& other) = delete;

	uint32_t Width() { return m_Width; }
	uint32_t Height() { return m_Height; }

	glm::vec4 Sample(glm::vec2 uv);
	glm::vec4 SamplePixel(glm::ivec2 pixel);
	void Write(glm::ivec2 pixel, glm::vec4 color);

	uint8_t* Data() { return m_Data; }
	void Clear(){
		uint32_t size = m_Width * m_Height * BytesPerPixel(m_Format);
		memset(m_Data, 0, size);
	}
	void Clear(glm::vec4 color);
	void Save(const std::string& path);
	
};