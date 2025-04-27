#pragma once
#include "Vector3.h"

namespace cl
{
	class Transform
	{
	public:
		Transform() : position({ 0.0f, 0.0f, 0.0f }), rotation({ 0.0f, 0.0f, 0.0f }), scale({ 1.0f, 1.0f, 1.0f }) {}
		Transform(const Vector3f& pos, const Vector3f& rot, const Vector3f& sc) : position(pos), rotation(rot), scale(sc) {}
	public:
		Vector3f position;
		Vector3f rotation;
		Vector3f scale;
	};
}