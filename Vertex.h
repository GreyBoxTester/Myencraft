#pragma once
#include "Vector2.h"
#include "Vector3.h"

namespace mc
{
	struct Vertex
	{
		cl::Vector3f pos;
		cl::Vector3f normal;
		cl::Vector2f tc;
	};

	struct WaterVertex
	{
		cl::Vector3f pos;
	};
}