#include "BVHNode.h"
#include<functional>
#include<algorithm>

static bool CompareX(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b)
{
    return a->GetBoundingBox().Min.x < a->GetBoundingBox().Max.x;
}

static bool CompareY(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b)
{
    return a->GetBoundingBox().Min.y < a->GetBoundingBox().Max.y;
}

static bool CompareZ(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b)
{
    return a->GetBoundingBox().Min.z < a->GetBoundingBox().Max.z;
}



BVHNode::BVHNode(const std::vector<std::shared_ptr<Shape>>& shapes)
    :BVHNode(shapes, 0, shapes.size())
{

}

BVHNode::BVHNode(const std::vector<std::shared_ptr<Shape>>& shapes, size_t begin, size_t end)
{
    for (int i = begin; i < end; i++)
    {
        m_BoundingBox.Add(shapes[i]->GetBoundingBox());
    };

    uint32_t count = end - begin;
    if (count == 1)
    {
        m_Left = shapes[begin];
        m_Right = shapes[begin];
    }
    else if (count == 2)
    {
        m_Left = shapes[begin];
        m_Right = shapes[begin+1];
    }
    else
    {
        glm::vec3 length = m_BoundingBox.Max - m_BoundingBox.Min;
        std::function<bool(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b)> compareFunc;
        if (length.x > length.y)
        {
            if (length.x > length.z)
                compareFunc = CompareX;
            else
                compareFunc = CompareZ;
        }
        else
        {
            if (length.y > length.z)
                compareFunc = CompareY;
            else
                compareFunc = CompareZ;
        }
        auto shapeCopy = shapes;
        std::sort(shapeCopy.begin() + begin, shapeCopy.begin() + end, compareFunc);
        m_Left = std::make_shared<BVHNode>(shapeCopy, begin, begin + count / 2);
        m_Right = std::make_shared<BVHNode>(shapeCopy, begin + count / 2, end);
    }
}

bool BVHNode::RayCheck(Ray& ray, HitRecord& hitRecord)
{
    if (!m_BoundingBox.RayCheck(ray, hitRecord.t))
        return false;
    bool left = m_Left->RayCheck(ray, hitRecord);
    bool right = m_Right->RayCheck(ray, hitRecord);
    return  left || right;
}
