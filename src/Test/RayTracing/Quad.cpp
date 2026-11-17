#include "Quad.h"
#include"../../RayTracing/Random.h"

Quad::Quad(const glm::vec3& o, const glm::vec3& u, const glm::vec3& v, std::shared_ptr<Material> mat)
    :m_Q(o), m_U(u), m_V(v), m_Mat(mat)
{
    auto n = glm::cross(u, v);
    m_Normal = glm::normalize(n);
    m_D = glm::dot(m_Normal, m_Q);
    m_W = n / glm::dot(n, n);
    m_Area = glm::length(n);

    m_BoundingBox.Add(o);
    m_BoundingBox.Add(o + u);
    m_BoundingBox.Add(o + v);
    m_BoundingBox.Add(o + u + v);
    m_BoundingBox.Padding(0.001);
}

float Quad::PdfValue(const glm::vec3& ori, const glm::vec3& dir)
{
    HitRecord record;
    record.minT = 0;
    Ray ray(ori, dir);
    if (!RayCheck(ray, record))
        return 0;
    float distance2 = record.t * record.t;
    float cosine = glm::abs(glm::dot(dir, record.normal));

    return distance2 / (cosine * m_Area);
}

glm::vec3 Quad::Random(const glm::vec3& ori)
{
    float u = Random01();
    float v = Random01();
    glm::vec3 p = m_Q + u * m_U + v * m_V;
    return glm::normalize(p - ori);
}

bool Quad::RayCheck(Ray& ray, HitRecord& hitRecord)
{
    auto denom = glm::dot(m_Normal, ray.Direction());

    if (denom == 0)
        return false;

    float t = (m_D - glm::dot(m_Normal, ray.Origin())) / denom;
    if (t < hitRecord.minT || t >= hitRecord.t)
        return false;

    glm::vec3 pos = ray.At(t);
    glm::vec3 v = pos - m_Q;
    auto alpha = glm::dot(m_W, glm::cross(v, m_V));
    auto beta = glm::dot(m_W, glm::cross(m_U, v));
    if (alpha < 0 || alpha>1 || beta < 0 || beta>1)
        return false;

    hitRecord.t = t;
    hitRecord.p = pos;
    hitRecord.frontFace = glm::dot(m_Normal, ray.Direction()) < 0;
    hitRecord.normal = glm::dot(m_Normal, ray.Direction()) < 0 ? m_Normal : -m_Normal;
    hitRecord.mat = m_Mat;
    hitRecord.uv = glm::vec2(alpha, beta);
    return true;
}
