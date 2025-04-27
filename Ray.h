#pragma once
#include "Vector2.h"
#include "Vector3.h"

namespace mc
{
	class Ray
	{
	public:
		Ray(const cl::Vector3f& startPos, const cl::Vector2f rot);
		cl::Vector3f step(float stepLength);
		float getLength() const;
	private:
		cl::Vector3f startPos;
		cl::Vector3f unitVec;
		float length;
	};
}
