#pragma once
#include "IChunkMap.h"
#include "Random.h"

namespace mc
{
	enum class StructureType : unsigned char
	{
		OakTree = 0u
	};

	void makeOakTree(IChunkMap* world, cl::Random& rand, const cl::Vector3i& pos);
}