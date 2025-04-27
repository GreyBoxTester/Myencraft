#pragma once
#include "Vector3.h"
#include "BlockConstants.h"

namespace mc
{
	class IChunkMap
	{
	public:
		virtual BlockType getBlock(const cl::Vector3i& pos) const = 0;
		virtual void setBlock(const cl::Vector3i& pos, BlockType type) = 0;
		virtual void setBlockNoChecks(const cl::Vector3i& pos, BlockType type) = 0;
	};
}