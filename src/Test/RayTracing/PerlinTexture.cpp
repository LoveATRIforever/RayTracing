#include "PerlinTexture.h"

glm::vec3 PerlinTexture::Sample(const glm::vec2& uv, const glm::vec3& point)
{
    return glm::vec3(1) * 0.5f * (glm::sin(point.z * m_Scale + 10 * glm::abs(m_Perlin.Turb(m_Scale * point, 7)))+1.0f);
}
