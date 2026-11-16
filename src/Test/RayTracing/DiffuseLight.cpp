#include "DiffuseLight.h"
#include"../../RayTracing/HitRecord.h"

bool DiffuseLight::Scatter(Ray& ray, HitRecord& hitRecord, glm::vec3& attenuation, Ray& scattered)
{
    return false;
}

glm::vec3 DiffuseLight::Emitted(HitRecord& hitRecord)
{
    return m_Emit->Sample(hitRecord.uv, hitRecord.p);
}
