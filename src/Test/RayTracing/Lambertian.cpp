#include "Lambertian.h"
#include"../../RayTracing/Random.h"
#include"../../RayTracing/HitRecord.h"
#include"../../RayTracing/Ray.h"

inline bool Vec3NearZero(glm::vec3 vec)
{
    const float zero = 1e-8;
    if (glm::abs(vec.x) < zero && glm::abs(vec.y) < zero && glm::abs(vec.z) < zero)
        return true;
    return false;
}

bool Lambertian::Scatter(Ray& ray, HitRecord& hitRecord, glm::vec3& attenuation, Ray& scattered)
{
    attenuation = m_Albedo->Sample(hitRecord.uv, hitRecord.p);
    glm::vec3 dir = RandomDirection() + hitRecord.normal;
    if (Vec3NearZero(dir))
    {
        dir = hitRecord.normal;
    }
    scattered = Ray(hitRecord.p, glm::normalize(dir));
    return true;
}
