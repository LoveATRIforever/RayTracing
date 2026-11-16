#include "Isotropic.h"
#include"../../RayTracing/Random.h"
#include"../../RayTracing/Ray.h"
#include"../../RayTracing/HitRecord.h"

bool Isotropic::Scatter(Ray& ray, HitRecord& hitRecord, glm::vec3& attenuation, Ray& scattered)
{
    scattered = Ray(hitRecord.p, RandomDirection());
    attenuation = m_Albedo;
    return true;
}
