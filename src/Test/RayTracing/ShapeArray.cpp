#include "ShapeArray.h"

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
