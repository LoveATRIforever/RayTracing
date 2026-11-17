#include "Pdfs.h"
#include"../../RayTracing/Random.h"

float SpherePdf::Value(const glm::vec3& dir)
{
    return 1 / (4 * 3.1416);
}

glm::vec3 SpherePdf::Gen()
{
    return RandomDirection();
}

float CosinePdf::Value(const glm::vec3& dir)
{
    return glm::dot(dir, m_Onb[2]) / 3.1416;
}

glm::vec3 CosinePdf::Gen()
{
    return m_Onb.ToGlobal(RandomCosineDirection());
}

float ShapePdf::Value(const glm::vec3& dir)
{
    
    return m_Shape->PdfValue(m_Ori, dir);
}

glm::vec3 ShapePdf::Gen()
{
    return m_Shape->Random(m_Ori);
}

float MixturePdf::Value(const glm::vec3& dir)
{
    return 0.5 * m_Pdf[0]->Value(dir) + 0.5f * m_Pdf[1]->Value(dir);
}

glm::vec3 MixturePdf::Gen()
{
    if (Random01() < 0.5f)
        return m_Pdf[0]->Gen();
    else
        return m_Pdf[1]->Gen();
}
