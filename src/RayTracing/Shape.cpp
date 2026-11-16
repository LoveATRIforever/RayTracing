#include "Shape.h"

bool BoundingBox::RayCheck(Ray& ray, float max)
{
    glm::vec3 ori = ray.Origin();
    glm::vec3 dir = ray.Direction();

    glm::vec2 t(0.001, max);
    for (int i = 0; i < 3; i++)
    {
        if (dir[i] == 0)
            continue;
        float k = 1 / dir[i];
        float t0 = (Min[i] - ori[i]) * k;
        float t1 = (Max[i] - ori[i]) * k;
        if (k < 0)
            std::swap(t0, t1);
        t[0] = glm::max(t[0], t0);
        t[1] = glm::min(t[1], t1);
    }
    if (t[0] < t[1])
        return true;
    return false;
}

void BoundingBox::Padding(float pad)
{
    glm::vec3 d = Max - Min;
    if (d.x == 0)
    {
        Max.x += pad;
        Min.x -= pad;
    }
    if (d.y == 0)
    {
        Max.y += pad;
        Min.y -= pad;
    }
    if (d.z == 0)
    {
        Max.z += pad;
        Min.z -= pad;
    }
}
