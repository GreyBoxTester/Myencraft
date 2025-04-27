#pragma once
#include "Vector3.h"

namespace mc
{
	class AABB
	{
	public:
		AABB();
		AABB(const cl::Vector3f& pos, const cl::Vector3f& size);
		bool isColliding(const AABB& other) const;
		void setPosition(const cl::Vector3f& pos);
		void setSize(const cl::Vector3f& size);
		const cl::Vector3f& getVMin() const;
		const cl::Vector3f& getVMax() const;
		cl::Vector3f getVP(const cl::Vector3f& normal) const;
		cl::Vector3f getVN(const cl::Vector3f& normal) const;
	private:
		cl::Vector3f vMin;
		cl::Vector3f vMax;
	};
}
