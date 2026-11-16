#include "Dielectric.h"
#include"../../RayTracing/Random.h"
#include<iostream>


inline float Reflectance(float cosine, float ref_idx) 
{
    // 使用 Schlick 的反射近似。
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * glm::pow((1 - cosine), 5);
}


bool Dielectric::Scatter(Ray& ray, HitRecord& hitRecord, glm::vec3& attenuation, Ray& scattered)
{
    attenuation = glm::vec3(1);
    float radio = glm::dot(hitRecord.normal, ray.Direction()) < 0 ? 1 / m_Ir : m_Ir;
    glm::vec3 v0 = (ray.Direction() - dot(ray.Direction(), hitRecord.normal) * hitRecord.normal) * radio;
    if (glm::dot(v0, v0) > 1 || Reflectance(glm::abs(glm::dot(hitRecord.normal, ray.Direction())), radio) > Random01())
    {
        glm::vec3 reflect = glm::vec3(-2 * glm::dot(ray.Direction(), hitRecord.normal)) * hitRecord.normal + ray.Direction();
        scattered = Ray(hitRecord.p, reflect);
        return true;
    }
    else
    {
        glm::vec3 v1;
        if (radio > 1)
        {
            v1 = glm::sqrt(1 - glm::dot(v0, v0)) * hitRecord.normal;
        }
        else
        {
            v1 = glm::sqrt(1 - glm::dot(v0, v0)) * -hitRecord.normal;
        }
        scattered = Ray(hitRecord.p, v1 + v0);
        return true;
    }
}
