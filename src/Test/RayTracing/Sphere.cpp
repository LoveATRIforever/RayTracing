#include "Sphere.h"
#include"../../RayTracing/Random.h"
#include"../../RayTracing/ONB.h"

glm::vec2 ComputeSphereUV(const glm::vec3 direction)
{
    const float pi = 3.1416;
    float u = std::atan2(-direction.z, direction.x) + pi;
    float v = std::acos(-direction.y);
    return { u / (2 * pi),v / pi };
}

Sphere::Sphere(glm::vec3 center, float radius, std::shared_ptr<Material> material)
    :m_Center(center), m_Radius(radius), m_Material(material)
{
    m_BoundingBox = { center - glm::vec3(radius),center + glm::vec3(radius) };
}

bool Sphere::RayCheck(Ray& ray, HitRecord& hitRecord)
{
    glm::vec3 co = ray.Origin() - m_Center;
    float a = 1;
    float h = glm::dot(ray.Direction(), co);
    float c = glm::dot(co, co) - m_Radius * m_Radius;

    float k = h * h - a * c;
    float t;
    if (k < 0)
    {
        return false;
    }
    else if (k == 0)
    {
        t = - h / a;
        if (t < hitRecord.minT || t >= hitRecord.t)
            return false;
    }
    else
    {
        float tmp = glm::sqrt(k);
        t = (-h - tmp) / a;
        if (t < hitRecord.minT || t >= hitRecord.t)
        {
            t = (-h + tmp) / a;
            if (t < hitRecord.minT || t >= hitRecord.t)
                return false;
        }
    }
    hitRecord.t = t;
    hitRecord.p = ray.At(t);
    hitRecord.normal = (hitRecord.p - m_Center) / m_Radius;
    hitRecord.mat = m_Material;
    hitRecord.uv = ComputeSphereUV(hitRecord.normal);
    return true;
}

float Sphere::PdfValue(const glm::vec3& ori, const glm::vec3& dir)
{
    HitRecord record;
    Ray ray(ori, dir);
    if (!RayCheck(ray, record))
        return 0;
    glm::vec3 tmp = m_Center - ori;
    float distance2 = glm::dot(tmp, tmp);
    return 1 / (2 * 3.1416 * (1 - glm::sqrt(1 - m_Radius * m_Radius / distance2)));
}

glm::vec3 Sphere::Random(const glm::vec3& ori)
{
    glm::vec3 dir = m_Center - ori;
    float distance2 = glm::dot(dir, dir);
    float t1 = Random01();
    float phi = t1 * 2 * 3.1416;
    float t2 = Random01();
    glm::vec3 res;
    res.z = 1 - (1 - glm::sqrt(1 - m_Radius * m_Radius / distance2)) * t2;
    float sin = glm::sqrt(1 - res.z * res.z);
    res.x = sin * glm::cos(phi);
    res.y = sin * glm::sin(phi);

    ONB onb(glm::normalize(dir));
    return onb.ToGlobal(res);
}
