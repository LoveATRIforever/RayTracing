#include "Lambertian.h"
#include"../../RayTracing/Random.h"
#include"../../RayTracing/HitRecord.h"
#include"../../RayTracing/Ray.h"
#include"Pdfs.h"

inline bool Vec3NearZero(glm::vec3 vec)
{
    const float zero = 1e-8;
    if (glm::abs(vec.x) < zero && glm::abs(vec.y) < zero && glm::abs(vec.z) < zero)
        return true;
    return false;
}

float Lambertian::ScatterPdf(Ray& ray, HitRecord& hitRecord, Ray& scattered)
{
    float cos = glm::dot(scattered.Direction(), hitRecord.normal);
    if (cos < 0)
        return 0;
    else
        return cos / 3.1416;
}

bool Lambertian::Scatter(Ray& ray, HitRecord& hitRecord, ScatterRecord& scatterRecord)
{
    scatterRecord.attenuation = m_Albedo->Sample(hitRecord.uv, hitRecord.p);
    scatterRecord.pdf = std::make_shared<CosinePdf>(hitRecord.normal);
    scatterRecord.skipPdf = false;
    return true;
}
