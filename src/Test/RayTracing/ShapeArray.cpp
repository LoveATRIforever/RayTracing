#include "ShapeArray.h"
#include"../../RayTracing/Random.h"

bool ShapeArray::RayCheck(Ray& ray, HitRecord& hitRecord)
{
    bool res = false;
    for (int i = 0; i < m_Array.size(); i++)
    {
        if (m_Array[i]->RayCheck(ray, hitRecord))
        {
            res = true;
        }
    }
    return res;
}

float ShapeArray::PdfValue(const glm::vec3& ori, const glm::vec3& dir)
{
    float weight = 1.0f / m_Array.size();
    float sum = 0;

    for (int i = 0; i < m_Array.size(); i++)
    {
        sum += weight * m_Array[i]->PdfValue(ori, dir);
    }
    return sum;
}

glm::vec3 ShapeArray::Random(const glm::vec3& ori)
{
    int size = m_Array.size();
    return m_Array[int(Random01() * size)]->Random(ori);
}
