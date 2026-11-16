#include "ImageTexture.h"

glm::vec3 ImageTexture::Sample(const glm::vec2& uv, const glm::vec3& point)
{
    return m_Image->Sample(uv);
}
