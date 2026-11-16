#include "Transform.h"

Translate::Translate(std::shared_ptr<Shape> shape, glm::vec3 translate)
    :m_Shape(shape), m_Translate(translate) 
{
    m_BoundingBox = BoundingBox(m_Shape->GetBoundingBox().Min + translate, m_Shape->GetBoundingBox().Max);
}

bool Translate::RayCheck(Ray& ray, HitRecord& hitRecord)
{
    Ray tmpRay(ray.Origin() - m_Translate, ray.Direction());
    if (!m_Shape->RayCheck(tmpRay, hitRecord))
        return false;
    hitRecord.p += m_Translate;
    return true;
}

RotateY::RotateY(std::shared_ptr<Shape> shape, float angle)
    :m_Shape(shape), m_Angle(angle) 
{
    float tmp = glm::radians(angle);
    m_Sin = glm::sin(tmp);
    m_Cos = glm::cos(tmp);

    glm::vec3 ps[2] = { m_Shape->GetBoundingBox().Min,m_Shape->GetBoundingBox().Max };
    for (int x = 0; x < 2; x++)
    {
        float vx = ps[x][0];
        for (int y = 0; y < 2; y++)
        {
            float vy = ps[y][1];
            for (int z = 0; z < 2; z++)
            {
                float vz = ps[z][2];
                glm::vec3 p(vx, vy, vz);
                glm::vec3 pR = p;
                pR.x = m_Cos * p.x + m_Sin * p.z;
                pR.z = -m_Sin * p.x + m_Cos * p.z;
                m_BoundingBox.Add(pR);
            }
        }
    }
}

bool RotateY::RayCheck(Ray& ray, HitRecord& hitRecord)
{
    glm::vec3 ori = ray.Origin();
    glm::vec3 dir = ray.Direction();

    glm::vec3 oriR = ori;
    oriR.x = m_Cos * ori.x - m_Sin * ori.z;
    oriR.z = m_Sin * ori.x + m_Cos * ori.z;
    glm::vec3 dirR = dir;
    dirR.x = m_Cos * dir.x - m_Sin * dir.z;
    dirR.z = m_Sin * dir.x + m_Cos * dir.z;

    Ray rayR(oriR, dirR);
    if (!m_Shape->RayCheck(rayR, hitRecord))
        return false;
    
    auto pR = hitRecord.p;
    glm::vec3 p = pR;
    p.x = m_Cos * pR.x + m_Sin * pR.z;
    p.z = -m_Sin * pR.x + m_Cos * pR.z;
    hitRecord.p = p;

    auto nR = hitRecord.normal;
    glm::vec3 n = nR;
    n.x = m_Cos * nR.x + m_Sin * nR.z;
    n.z = -m_Sin * nR.x + m_Cos * nR.z;
    hitRecord.normal = n;

    return true;
}
