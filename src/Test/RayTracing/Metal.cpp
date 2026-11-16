#include "Metal.h"
#include"../../RayTracing/HitRecord.h"
#include"../../RayTracing/Ray.h"
#include"../../RayTracing/Random.h"

bool Metal::Scatter(Ray& ray, HitRecord& hitRecord, glm::vec3& attenuation, Ray& scattered)
{
    attenuation = m_Albedo;
    glm::vec3 reflect = glm::vec3(-2 * glm::dot(ray.Direction(), hitRecord.normal)) * hitRecord.normal + ray.Direction();
    scattered = Ray(hitRecord.p, glm::normalize(reflect + glm::vec3(m_Fuzz) * RandomDirection()));
    return glm::dot(hitRecord.normal, scattered.Direction()) > 0;
}
