#include "Ray.h"

mc::Ray::Ray(const cl::Vector3f& startPos, const cl::Vector2f rot)
    : startPos(startPos), unitVec(-sinf(rot.y) * cosf(rot.x), sinf(rot.x), cosf(rot.y) * cosf(rot.x)), length(0)
{}

cl::Vector3f mc::Ray::step(float stepLength)
{
    length += stepLength;
    return startPos + (unitVec * length);
}

float mc::Ray::getLength() const
{
    return length;
}
