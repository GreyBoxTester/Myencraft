#include "AABB.h"

float min(float a, float b)
{
    return a < b ? a : b;
}

mc::AABB::AABB()
    : vMin(), vMax(1.0f, 1.0f, 1.0f)
{}

mc::AABB::AABB(const cl::Vector3f& pos, const cl::Vector3f& size)
    : vMin(pos), vMax(pos + size)
{}

bool mc::AABB::isColliding(const mc::AABB& other) const
{
    return (vMin.x < other.vMax.x && vMax.x > other.vMin.x) &&
           (vMin.y < other.vMax.y && vMax.y > other.vMin.y) &&
           (vMin.z < other.vMax.z && vMax.z > other.vMin.z);
}

void mc::AABB::setPosition(const cl::Vector3f& pos)
{
    vMax += pos - vMin;
    vMin = pos;
}

void mc::AABB::setSize(const cl::Vector3f& size)
{
    vMax = vMin + size;
}

const cl::Vector3f& mc::AABB::getVMin() const
{
    return vMin;
}

const cl::Vector3f& mc::AABB::getVMax() const
{
    return vMax;
}

cl::Vector3f mc::AABB::getVP(const cl::Vector3f& normal) const
{
    cl::Vector3f p = vMin;
    if (normal.x > 0.0f) { p.x = vMax.x; }
    if (normal.y > 0.0f) { p.y = vMax.y; }
    if (normal.z > 0.0f) { p.z = vMax.z; }
    return p;
}

cl::Vector3f mc::AABB::getVN(const cl::Vector3f& normal) const
{
    cl::Vector3f n = vMin;
    if (normal.x > 0.0f) { n.x = vMin.x; }
    if (normal.y > 0.0f) { n.y = vMin.y; }
    if (normal.z > 0.0f) { n.z = vMin.z; }
    return n;
}
