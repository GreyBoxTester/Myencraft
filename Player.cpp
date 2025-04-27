#include "Player.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Ray.h"

#define RUN_ACCEL 20.0f
#define WALK_ACCEL 14.0f
#define SNEAK_ACCEL 4.3f
#define JUMP_VEL 9.5f
#define SWIM_UP_ACCEL 29.0f

mc::Player::Player(const mc::Position& pos)
	: Entity(pos, {0.6f, 1.8f, 0.6f})
{}

void mc::Player::handleInput(cl::Graphics& gfx, cl::Vector2f& mouseMoveDelta, World& world)
{
	static bool lastLMBState = false;
	static bool lastRMBState = false;
	static bool lastMMBState = false;
	static bool lastLShiftState = false;

	mouseMoveDelta *= 0.005f;
	rot.x -= mouseMoveDelta.y;
	rot.y -= mouseMoveDelta.x;
	if (rot.x > 1.5707963f) { rot.x = 1.5707963f; }
	if (rot.x < -1.5707963f) { rot.x = -1.5707963f; }
	if (rot.y > 6.283185307f) { rot.y -= 6.283185307f; }
	if (rot.y < 0.0f) { rot.y += 6.283185307f; }

	//=====movement======
	if (cl::Keyboard::isKeyPressed(VK_LSHIFT))
	{
		if (cl::Keyboard::isKeyPressed('W')) { accel.z += SNEAK_ACCEL * cosf(rot.y); accel.x -= SNEAK_ACCEL * sinf(rot.y); }
		if (cl::Keyboard::isKeyPressed('S')) { accel.z -= SNEAK_ACCEL * cosf(rot.y); accel.x += SNEAK_ACCEL * sinf(rot.y); }
		if (cl::Keyboard::isKeyPressed('A')) { accel.x -= SNEAK_ACCEL * cosf(rot.y); accel.z -= SNEAK_ACCEL * sinf(rot.y); }
		if (cl::Keyboard::isKeyPressed('D')) { accel.x += SNEAK_ACCEL * cosf(rot.y); accel.z += SNEAK_ACCEL * sinf(rot.y); }
	}
	else if (cl::Keyboard::isKeyPressed(VK_LCONTROL))
	{
		if (cl::Keyboard::isKeyPressed('W')) { accel.z += RUN_ACCEL * cosf(rot.y); accel.x -= RUN_ACCEL * sinf(rot.y); }
		if (cl::Keyboard::isKeyPressed('S')) { accel.z -= RUN_ACCEL * cosf(rot.y); accel.x += RUN_ACCEL * sinf(rot.y); }
		if (cl::Keyboard::isKeyPressed('A')) { accel.x -= RUN_ACCEL * cosf(rot.y); accel.z -= RUN_ACCEL * sinf(rot.y); }
		if (cl::Keyboard::isKeyPressed('D')) { accel.x += RUN_ACCEL * cosf(rot.y); accel.z += RUN_ACCEL * sinf(rot.y); }
	}
	else
	{
		if (cl::Keyboard::isKeyPressed('W')) { accel.z += WALK_ACCEL * cosf(rot.y); accel.x -= WALK_ACCEL * sinf(rot.y); }
		if (cl::Keyboard::isKeyPressed('S')) { accel.z -= WALK_ACCEL * cosf(rot.y); accel.x += WALK_ACCEL * sinf(rot.y); }
		if (cl::Keyboard::isKeyPressed('A')) { accel.x -= WALK_ACCEL * cosf(rot.y); accel.z -= WALK_ACCEL * sinf(rot.y); }
		if (cl::Keyboard::isKeyPressed('D')) { accel.x += WALK_ACCEL * cosf(rot.y); accel.z += WALK_ACCEL * sinf(rot.y); }
	}

	if (cl::Keyboard::isKeyPressed(VK_SPACE)) 
	{
		if (inWater)
		{
			accel.y += SWIM_UP_ACCEL;
		}
		if (onGround)
		{
			vel.y = JUMP_VEL;
		}
	}
	

	//=====shifting_collider_modifications======
	if (!cl::Keyboard::isKeyPressed(VK_LSHIFT) && lastLShiftState)
	{
		pos.p.y += 0.2f;
		collider.setPosition(pos.p - centerOffset);
		setColliderSize({ 0.6f, 1.8f, 0.6f });
	}
	else if (cl::Keyboard::isKeyPressed(VK_LSHIFT) && !lastLShiftState)
	{
		setColliderSize({ 0.6f, 1.5f, 0.6f });
	}
	if (cl::Keyboard::isKeyPressed(VK_LSHIFT) && inWater) { accel.y -= SWIM_UP_ACCEL; }
	lastLShiftState = cl::Keyboard::isKeyPressed(VK_LSHIFT);

	//=====terrain_modification=====
	if (cl::Mouse::isPressed(cl::Mouse::Button::Left) && !lastLMBState)
	{
		mc::Ray ray({ pos.p.x, pos.p.y + 0.72f, pos.p.z }, rot);
		while (ray.getLength() < 5.0f)
		{
			cl::Vector3f posf = ray.step(0.1f);
			cl::Vector3i blockPos(floor(posf.x) + pos.getChunkPos().x, floor(posf.y), floor(posf.z) + pos.getChunkPos().y);

			if (NOT_IGNORED_BY_RAY(world.getBlock(blockPos)))
			{
				world.setBlock(blockPos, mc::BlockType::Air);
				world.updateMeshes(gfx);
				break;
			}
		}
	}

	lastLMBState = cl::Mouse::isPressed(cl::Mouse::Button::Left);

	if (cl::Mouse::isPressed(cl::Mouse::Button::Right) && !lastRMBState)
	{
		mc::Ray ray({ pos.p.x, pos.p.y + 0.72f, pos.p.z }, rot);
		while (ray.getLength() < 5.0f)
		{
			cl::Vector3f posf = ray.step(0.1f);
			cl::Vector3i blockPos(floor(posf.x) + pos.getChunkPos().x, floor(posf.y), floor(posf.z) + pos.getChunkPos().y);

			if (NOT_IGNORED_BY_RAY(world.getBlock(blockPos)))
			{
				posf = ray.step(-0.1f);
				blockPos.x = (int)floor(posf.x) + pos.getChunkPos().x;
				blockPos.y = (int)floor(posf.y);
				blockPos.z = (int)floor(posf.z) + pos.getChunkPos().y;
				if (IS_FULL_BLOCK(selectedBlock))
				{
					mc::AABB block({floor(posf.x), floor(posf.y), floor(posf.z)}, { 1.0f,1.0f,1.0f });
					if (!collider.isColliding(block))
					{
						world.setBlock(blockPos, selectedBlock);
						world.updateMeshes(gfx);
					}
				}
				else
				{
					world.setBlock(blockPos, selectedBlock);
					world.updateMeshes(gfx);
				}

				break;
			}
		}
	}
	lastRMBState = cl::Mouse::isPressed(cl::Mouse::Button::Right);

	if (cl::Mouse::isPressed(cl::Mouse::Button::Middle) && !lastMMBState)
	{
		mc::Ray ray({ pos.p.x, pos.p.y + 0.72f, pos.p.z }, rot);
		while (ray.getLength() < 5.0f)
		{
			cl::Vector3f posf = ray.step(0.1f);
			cl::Vector3i blockPos(floor(posf.x) + pos.getChunkPos().x, floor(posf.y), floor(posf.z) + pos.getChunkPos().y);

			if (NOT_IGNORED_BY_RAY(world.getBlock(blockPos)))
			{
				selectedBlock = world.getBlock(blockPos);
				break;
			}
		}
	}
	lastMMBState = cl::Mouse::isPressed(cl::Mouse::Button::Middle);
}
