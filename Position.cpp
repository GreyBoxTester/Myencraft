#include "Position.h"

mc::Position::Position()
    : p(), relativeChunkPos()
{}

mc::Position::Position(float _x, float _y, float _z)
    : p(_x, _y, _z), relativeChunkPos()
{
    update();
}

mc::Position::Position(const cl::Vector3f& pos)
    : p(pos.x, pos.y, pos.z), relativeChunkPos()
{
    update();
}

mc::Position::Position(const cl::Vector3f& pos, const cl::Vector2i& relativeChunkPos)
    : p(pos), relativeChunkPos(relativeChunkPos)
{}

void mc::Position::update()
{
    if (p.x > 16.0f)
    {
        int n = (int)(((unsigned int)floorf(p.x)) & (~0xf));
        relativeChunkPos.x += n;
        p.x -= (float)n;
    }
    else if (p.x < 0.0f)
    {
        int n = (int)(((unsigned int)floorf(-p.x) + 16u) & (~0xf));
        relativeChunkPos.x -= n;
        p.x += (float)n;
    }

    if (p.z > 16.0f)
    {
        int n = (int)(((unsigned int)floorf(p.z)) & (~0xf));
        relativeChunkPos.y += n;
        p.z -= (float)n;
    }
    else if (p.z < 0.0f)
    {
        int n = (int)(((unsigned int)floorf(-p.z) + 16u) & (~0xf));
        relativeChunkPos.y -= n;
        p.z += (float)n;
    }
}

void mc::Position::getWorldPos(cl::Vector3f* posOut) const
{
    posOut->x = p.x + (float)relativeChunkPos.x;
    posOut->y = p.y;
    posOut->z = p.z + (float)relativeChunkPos.y;
}

void mc::Position::getWorldPos(cl::Vector3i* posOut) const
{
    posOut->x = (int)p.x + relativeChunkPos.x;
    posOut->y = (int)p.y;
    posOut->z = (int)p.z + relativeChunkPos.y;
}

const cl::Vector2i& mc::Position::getChunkPos() const
{
    return relativeChunkPos;
}
