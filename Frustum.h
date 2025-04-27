#pragma once
#include "Vector3.h"
#include "AABB.h"
#include <DirectXMath.h>

namespace dx = DirectX;

namespace mc
{
	enum class FrustumPlaneID : unsigned char
	{
		Left = 0u,
		Right,
		Top,
		Bottom,
		Near,
		Far
	};

	struct FrustumPlane
	{
		cl::Vector3f normal;
		float distance;
	};

	class Frustum
	{
	public:
		void update(const dx::XMMATRIX& viewProjMat);
		bool isPointInFrustum(const cl::Vector3f& pt);
		bool isSphereInFrustum(const cl::Vector3f& pt, float rad);
		bool isBoxInFrustum(const AABB& box);
	private:
		FrustumPlane planes[6];
	};
}
