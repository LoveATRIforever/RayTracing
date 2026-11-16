#pragma once
#include"../../RayTracing/RTexture.h"
#include"../../Rasterize/Texture.h"
#include<memory>

class ImageTexture :public RTexture
{
private:
	std::shared_ptr<Texture> m_Image;
public:
	ImageTexture(const std::string& path)
		:m_Image(std::make_shared<Texture>(path)){ }

	glm::vec3 Sample(const glm::vec2& uv, const glm::vec3& point) override;

};