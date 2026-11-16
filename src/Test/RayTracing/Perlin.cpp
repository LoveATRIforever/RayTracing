#include "Perlin.h"
#include"../../RayTracing/Random.h"
#include<algorithm>

static void Permute(int* p, int n)
{
    for (int i = n - 1; i > 0; i--) {
        int target = int(Random01() * (i + 1));
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
}

Perlin::Perlin()
{
    for (int i = 0; i < m_RanFloat.size(); i++)
    {
        m_RanFloat[i] = RandomDirection();
        m_X[i] = i;
        m_Y[i] = i;
        m_Z[i] = i;
    }
    Permute(&m_X[0], m_X.size());
    Permute(&m_Y[0], m_Y.size());
    Permute(&m_Z[0], m_Z.size());
}

float Perlin::Noise(const glm::vec3& point)
{
    glm::vec3 uvw = point - glm::floor(point);
    glm::vec3 uvwSmooth;
    glm::ivec3 ijk = glm::floor(point);
    for (int i = 0; i < 3; i++)
        uvwSmooth[i] = uvw[i] * uvw[i] * (3 - 2 * uvw[i]);
    glm::vec3 val[2][2][2];
    for (int dx = 0; dx < 2; dx++)
    {
        for (int dy = 0; dy < 2; dy++)
        {
            for (int dz = 0; dz < 2; dz++)
            {
                val[dx][dy][dz] = m_RanFloat[
                    m_X[(ijk[0] + dx) & 255] ^
                    m_Y[(ijk[1] + dy) & 255] ^
                    m_Z[(ijk[2] + dz) & 255]
                ];
            }
        }
    }
    
    auto accum = 0.0;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
            {
                glm::vec3 weight = uvw - glm::vec3(i, j, k);
                accum += (i * uvwSmooth[0] + (1 - i) * (1 - uvwSmooth[0])) *
                    (j * uvwSmooth[1] + (1 - j) * (1 - uvwSmooth[1])) *
                    (k * uvwSmooth[2] + (1 - k) * (1 - uvwSmooth[2])) * glm::dot(val[i][j][k], weight);
            }

    return accum;
}

float Perlin::Turb(const glm::vec3& point, int depth)
{
    float res = 0;
    glm::vec3 p = point;
    float k = 1;
    for (int i = 0; i < depth; i++)
    {
        res += k * Noise(p);
        k *= 0.5;
        p *= 2;
    }
    return res;
}

