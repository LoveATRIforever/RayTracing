#include "Isotropic.h"
#include"../../RayTracing/Random.h"
#include"../../RayTracing/Ray.h"
#include"../../RayTracing/HitRecord.h"
#include"Pdfs.h"

float Isotropic::ScatterPdf(Ray& ray, HitRecord& hitRecord, Ray& scattered)
{
    return 1 / (4 * 3.1416);
}

bool Isotropic::Scatter(Ray& ray, HitRecord& hitRecord, ScatterRecord& scatterRecord)
{
    scatterRecord.attenuation = m_Albedo;
    scatterRecord.pdf = std::make_shared<SpherePdf>();
    scatterRecord.skipPdf = false;
    return true;
}
