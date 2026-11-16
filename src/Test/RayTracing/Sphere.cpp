#include "Sphere.h"

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
