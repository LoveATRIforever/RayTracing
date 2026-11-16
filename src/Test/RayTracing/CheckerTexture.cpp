#include "CheckerTexture.h"

glm::vec3 CheckerTexture::Sample(const glm::vec2& uv, const glm::vec3& point)
{
    glm::ivec3 iPoint = glm::floor(point * m_Scale);

    bool choose = (iPoint.x + iPoint.y + iPoint.z) % 2 == 0;
    if (choose)
        return m_Color1;
    else
        return m_Color2;
}
