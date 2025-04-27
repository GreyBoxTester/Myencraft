#pragma once
#include "Entity.h"
#include "World.h"

namespace mc
{
	class Player : public Entity
	{
	public:
		Player(const Position& pos);
		void handleInput(cl::Graphics& gfx, cl::Vector2f& mouseMoveDelta, World& world);
	private:
		BlockType selectedBlock = mc::BlockType::OakLeaves;
	};
}
