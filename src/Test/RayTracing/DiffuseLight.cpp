#include "DiffuseLight.h"
#include"../../RayTracing/HitRecord.h"
#include"../../RayTracing/Ray.h"

bool DiffuseLight::Scatter(Ray& ray, HitRecord& hitRecord, ScatterRecord& scatterRecord)
{
    return false;
}

glm::vec3 DiffuseLight::Emitted(Ray& ray, HitRecord& hitRecord)
{
    if (hitRecord.frontFace)
        return m_Emit->Sample(hitRecord.uv, hitRecord.p);
    else
        return glm::vec3(0);
}
