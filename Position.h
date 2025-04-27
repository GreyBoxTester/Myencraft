#pragma once
#include "Vector3.h"
#include "Vector2.h"

namespace mc
{
	class Position
	{
	public:
		Position();
		Position(float _x, float _y, float _z);
		Position(const cl::Vector3f& pos);
		Position(const cl::Vector3f& pos, const cl::Vector2i& relativeChunkPos);
		void update();
		void getWorldPos(cl::Vector3f* posOut) const;
		void getWorldPos(cl::Vector3i* posOut) const;
		const cl::Vector2i& getChunkPos() const;
	public:
		cl::Vector3f p;
	private:
		cl::Vector2i relativeChunkPos;
	};
}
