#include "Frustum.h"

#define GET_BY_INDEX(matrix,x,y) matrix.r[x].m128_f32[y]

void mc::Frustum::update(const dx::XMMATRIX& viewProjMat)
{
    planes[(size_t)FrustumPlaneID::Left].normal.x = GET_BY_INDEX(viewProjMat, 0, 3) + GET_BY_INDEX(viewProjMat, 0, 0);
    planes[(size_t)FrustumPlaneID::Left].normal.y = GET_BY_INDEX(viewProjMat, 1, 3) + GET_BY_INDEX(viewProjMat, 1, 0);
    planes[(size_t)FrustumPlaneID::Left].normal.z = GET_BY_INDEX(viewProjMat, 2, 3) + GET_BY_INDEX(viewProjMat, 2, 0);
    planes[(size_t)FrustumPlaneID::Left].distance = GET_BY_INDEX(viewProjMat, 3, 3) + GET_BY_INDEX(viewProjMat, 3, 0);

    planes[(size_t)FrustumPlaneID::Right].normal.x = GET_BY_INDEX(viewProjMat, 0, 3) - GET_BY_INDEX(viewProjMat, 0, 0);
    planes[(size_t)FrustumPlaneID::Right].normal.y = GET_BY_INDEX(viewProjMat, 1, 3) - GET_BY_INDEX(viewProjMat, 1, 0);
    planes[(size_t)FrustumPlaneID::Right].normal.z = GET_BY_INDEX(viewProjMat, 2, 3) - GET_BY_INDEX(viewProjMat, 2, 0);
    planes[(size_t)FrustumPlaneID::Right].distance = GET_BY_INDEX(viewProjMat, 3, 3) - GET_BY_INDEX(viewProjMat, 3, 0);

    planes[(size_t)FrustumPlaneID::Top].normal.x = GET_BY_INDEX(viewProjMat, 0, 3) - GET_BY_INDEX(viewProjMat, 0, 1);
    planes[(size_t)FrustumPlaneID::Top].normal.y = GET_BY_INDEX(viewProjMat, 1, 3) - GET_BY_INDEX(viewProjMat, 1, 1);
    planes[(size_t)FrustumPlaneID::Top].normal.z = GET_BY_INDEX(viewProjMat, 2, 3) - GET_BY_INDEX(viewProjMat, 2, 1);
    planes[(size_t)FrustumPlaneID::Top].distance = GET_BY_INDEX(viewProjMat, 3, 3) - GET_BY_INDEX(viewProjMat, 3, 1);

    planes[(size_t)FrustumPlaneID::Bottom].normal.x = GET_BY_INDEX(viewProjMat, 0, 3) + GET_BY_INDEX(viewProjMat, 0, 1);
    planes[(size_t)FrustumPlaneID::Bottom].normal.y = GET_BY_INDEX(viewProjMat, 1, 3) + GET_BY_INDEX(viewProjMat, 1, 1);
    planes[(size_t)FrustumPlaneID::Bottom].normal.z = GET_BY_INDEX(viewProjMat, 2, 3) + GET_BY_INDEX(viewProjMat, 2, 1);
    planes[(size_t)FrustumPlaneID::Bottom].distance = GET_BY_INDEX(viewProjMat, 3, 3) + GET_BY_INDEX(viewProjMat, 3, 1);

    planes[(size_t)FrustumPlaneID::Near].normal.x = GET_BY_INDEX(viewProjMat, 0, 3) + GET_BY_INDEX(viewProjMat, 0, 2);
    planes[(size_t)FrustumPlaneID::Near].normal.y = GET_BY_INDEX(viewProjMat, 1, 3) + GET_BY_INDEX(viewProjMat, 1, 2);
    planes[(size_t)FrustumPlaneID::Near].normal.z = GET_BY_INDEX(viewProjMat, 2, 3) + GET_BY_INDEX(viewProjMat, 2, 2);
    planes[(size_t)FrustumPlaneID::Near].distance = GET_BY_INDEX(viewProjMat, 3, 3) + GET_BY_INDEX(viewProjMat, 3, 2);

    planes[(size_t)FrustumPlaneID::Far].normal.x = GET_BY_INDEX(viewProjMat, 0, 3) - GET_BY_INDEX(viewProjMat, 0, 2);
    planes[(size_t)FrustumPlaneID::Far].normal.y = GET_BY_INDEX(viewProjMat, 1, 3) - GET_BY_INDEX(viewProjMat, 1, 2);
    planes[(size_t)FrustumPlaneID::Far].normal.z = GET_BY_INDEX(viewProjMat, 2, 3) - GET_BY_INDEX(viewProjMat, 2, 2);
    planes[(size_t)FrustumPlaneID::Far].distance = GET_BY_INDEX(viewProjMat, 3, 3) - GET_BY_INDEX(viewProjMat, 3, 2);

    for (unsigned int i = 0; i < 6u; i++)
    {
        float l = cl::length(planes[i].normal);
        planes[i].normal /= l;
        planes[i].distance /= l;
    }
}

bool mc::Frustum::isPointInFrustum(const cl::Vector3f& pt)
{
    for (unsigned int i = 0; i < 6u; i++)
    {
        if (cl::dotProduct(planes[i].normal, pt) + planes[i].distance < 0) { return false; }
    }

    return true;
}

bool mc::Frustum::isSphereInFrustum(const cl::Vector3f& pt, float rad)
{
    for (unsigned int i = 0; i < 6u; i++)
    {
        if (cl::dotProduct(planes[i].normal, pt) + planes[i].distance + rad < 0) { return false; }
    }

    return true;
}

bool mc::Frustum::isBoxInFrustum(const AABB& box)
{
    for (unsigned int i = 0; i < 6u; i++)
    {
        if (cl::dotProduct(planes[i].normal, box.getVP(planes[i].normal)) + planes[i].distance < 0) { return false; }
    }

    return true;
}
