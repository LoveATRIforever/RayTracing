#include "ConstantMedium.h"
#include"../../RayTracing/Random.h"


bool ConstantMedium::RayCheck(Ray& ray, HitRecord& hitRecord)
{

    HitRecord record1, record2;
    record1.minT = -std::numeric_limits<float>::infinity();
    if (!m_Shape->RayCheck(ray, record1))
        return false;
    record2.minT = record1.t + 0.0001;
    if (!m_Shape->RayCheck(ray, record2))
        return false;

    if (record1.t < hitRecord.minT)
        record1.t = hitRecord.minT;
    if (record2.t > hitRecord.t)
        record2.t = hitRecord.t;

    if (record1.t >= record2.t)
        return false;

    float rayLength = record2.t - record1.t;
    float distance = m_C * std::log(Random01());

    if (distance >= rayLength)
        return false;

    hitRecord.t = record1.t + distance;
    hitRecord.p = ray.At(hitRecord.t);
    hitRecord.mat = m_Mat;

    return true;
}
