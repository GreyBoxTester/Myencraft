#include "Entity.h"

#define MC_G 28.449f
#define MC_G_IN_WATER 5.0f

mc::Entity::Entity(const mc::Position& pos, const cl::Vector3f& colliderSize)
	: pos(pos), centerOffset(colliderSize / 2.0f), collider(pos.p - centerOffset, colliderSize), onGround(false), inWater(false)
{}

void mc::Entity::update(const mc::World& world, float dt)
{
	onGround = false;
	float time = 0.0f;
	if (inWater) { vel.y -= MC_G_IN_WATER * dt; }
	else { vel.y -= MC_G * dt; }
	inWater = false;
	vel += accel * dt;
	accel = { 0.0f,0.0f,0.0f };

	for (int i = 0; i < 3 && time < 1.0f; i++)
	{
		time = collide(world, time, dt);
	}

	cl::Vector3i worldPos;
	pos.getWorldPos(&worldPos);
	if (world.getBlock(worldPos) == mc::BlockType::Water)
	{
		inWater = true;
		vel.y *= powf(0.93f, dt * 62.5f);
		vel.x *= powf(0.80f, dt * 62.5f);
		vel.z *= powf(0.80f, dt * 62.5f);
	}
	else
	{
		vel.y *= powf(0.99f, dt * 62.5f);
		if (onGround)
		{
			vel.x *= powf(0.92f, dt * 62.5f);
			vel.z *= powf(0.92f, dt * 62.5f);
		}
		else
		{
			vel.x *= powf(0.96f, dt * 62.5f);
			vel.z *= powf(0.96f, dt * 62.5f);
		}
	}
}

float mc::Entity::collide(const mc::World& world, float time, float dt)
{
	cl::Vector3i worldPos;
	pos.getWorldPos(&worldPos);
	cl::Vector3i dest = worldPos + cl::Vector3i(vel * (1.0f - time) * dt);
	cl::Vector3f hitNormal;
	cl::Vector3i minBlock(
		(min(worldPos.x, dest.x) - (int)centerOffset.x - 2),
		(min(worldPos.y, dest.y) - (int)centerOffset.y - 2),
		(min(worldPos.z, dest.z) - (int)centerOffset.z - 2)
	);
	cl::Vector3i maxBlock(
		(max(worldPos.x, dest.x) + (int)centerOffset.x + 2),
		(max(worldPos.y, dest.y) + (int)centerOffset.y + 2),
		(max(worldPos.z, dest.z) + (int)centerOffset.z + 2)
	);

	mc::AABB block({}, { 1.0f,1.0f,1.0f });

	float smallestTime = 1.0f;

	for (int x = minBlock.x; x < maxBlock.x; x++)
	{
		for (int y = minBlock.y; y < maxBlock.y; y++)
		{
			for (int z = minBlock.z; z < maxBlock.z; z++)
			{
				if (IS_FULL_BLOCK(world.getBlock({ x,y,z })))
				{
					block.setPosition(cl::Vector3f((float)(x - pos.getChunkPos().x), (float)y, (float)(z - pos.getChunkPos().y)));

					cl::Vector3f distEntry, distExit, entry, exit;
					calculateEntryExit(block, vel * dt, &distEntry, &distExit, &entry, &exit);

					float entryTime = fmaxf(fmaxf(entry.x, entry.y), entry.z);
					float exitTime = fminf(fminf(exit.x, exit.y), exit.z);

					if (!(entryTime > exitTime || entry.x < 0.0f && entry.y < 0.0f && entry.z < 0.0f || entry.x > 1.0f || entry.y > 1.0f || entry.z > 1.0f) && smallestTime > entryTime)
					{
						smallestTime = entryTime;
						calculateHitNormal(entry, distEntry, &hitNormal);
					}
				}

				/*else if (blockType == mc::BlockType::Water)
				{
					block.setPosition(cl::Vector3f(x, y, z));

					cl::Vector3f distEntry, distExit, entry, exit;
					calculateEntryExit(block, vel * dt, &distEntry, &distExit, &entry, &exit);

					float entryTime = fmaxf(fmaxf(entry.x, entry.y), entry.z);
					float exitTime = fminf(fminf(exit.x, exit.y), exit.z);

					if (!(entryTime > exitTime || entry.x > 1.0f || entry.y > 1.0f || entry.z > 1.0f))
					{
						if (exitTime > 1.0f) { inWater = true; }
					}
				}*/
			}
		}
	}

	pos.p += vel * (dt * smallestTime);

	/*if (hitNormal.x != 0.0f) { pos.p.x -= vel.x * dt; vel.x = hitNormal.x * dt; }
	if (hitNormal.z != 0.0f) { pos.p.z -= vel.z * dt; vel.z = hitNormal.z * dt; }
	if (hitNormal.y > 0.0f) { vel.y = dt * 0.33f; pos.p.y += MC_G * dt * dt; onGround = true; }
	else if (hitNormal.y < 0.0f) { pos.p.y -= vel.y * dt; vel.y = -dt; }*/

	if (hitNormal.x != 0.0f) { pos.p.x -= vel.x * 0.008f; vel.x = hitNormal.x * 0.008f; }
	if (hitNormal.z != 0.0f) { pos.p.z -= vel.z * 0.008f; vel.z = hitNormal.z * 0.008f; }
	if (hitNormal.y > 0.0f) { vel.y = 0.00264f; pos.p.y += 0.0018207f; onGround = true; }
	else if (hitNormal.y < 0.0f) { pos.p.y -= vel.y * dt; vel.y = -0.008f; }

	pos.update();

	collider.setPosition(pos.p - centerOffset);

	return time + smallestTime;
}

void mc::Entity::calculateEntryExit(const mc::AABB& block, const cl::Vector3f& velocity, cl::Vector3f* distEntryOut, cl::Vector3f* distExitOut, cl::Vector3f* entryOut, cl::Vector3f* exitOut)
{
	if (velocity.x > 0.0f)
	{
		distEntryOut->x = block.getVMin().x - collider.getVMax().x;
		distExitOut->x = block.getVMax().x - collider.getVMin().x;
	}
	else
	{
		distEntryOut->x = block.getVMax().x - collider.getVMin().x;
		distExitOut->x = block.getVMin().x - collider.getVMax().x;
	}

	if (velocity.y > 0.0f)
	{
		distEntryOut->y = block.getVMin().y - collider.getVMax().y;
		distExitOut->y = block.getVMax().y - collider.getVMin().y;
	}
	else
	{
		distEntryOut->y = block.getVMax().y - collider.getVMin().y;
		distExitOut->y = block.getVMin().y - collider.getVMax().y;
	}

	if (velocity.z > 0.0f)
	{
		distEntryOut->z = block.getVMin().z - collider.getVMax().z;
		distExitOut->z = block.getVMax().z - collider.getVMin().z;
	}
	else
	{
		distEntryOut->z = block.getVMax().z - collider.getVMin().z;
		distExitOut->z = block.getVMin().z - collider.getVMax().z;
	}

	if (velocity.x != 0.0f)
	{
		entryOut->x = distEntryOut->x / velocity.x;
		exitOut->x = distExitOut->x / velocity.x;
	}
	else
	{
		entryOut->x = -std::numeric_limits<float>::infinity();
		exitOut->x = std::numeric_limits<float>::infinity();
	}

	if (velocity.y != 0.0f)
	{
		entryOut->y = distEntryOut->y / velocity.y;
		exitOut->y = distExitOut->y / velocity.y;
	}
	else
	{
		entryOut->y = -std::numeric_limits<float>::infinity();
		exitOut->y = std::numeric_limits<float>::infinity();
	}

	if (velocity.z != 0.0f)
	{
		entryOut->z = distEntryOut->z / velocity.z;
		exitOut->z = distExitOut->z / velocity.z;
	}
	else
	{
		entryOut->z = -std::numeric_limits<float>::infinity();
		exitOut->z = std::numeric_limits<float>::infinity();
	}
}

void mc::Entity::calculateHitNormal(const cl::Vector3f& entry, const cl::Vector3f& distEntry, cl::Vector3f* normalOut)
{
	if (entry.x > entry.y && entry.x > entry.z)
	{
		if (distEntry.x < 0.0f)
		{
			normalOut->x = 1.0f;
			normalOut->y = 0.0f;
			normalOut->z = 0.0f;
		}
		else
		{
			normalOut->x = -1.0f;
			normalOut->y = 0.0f;
			normalOut->z = 0.0f;
		}
	}
	else if (entry.y > entry.x && entry.y > entry.z)
	{
		if (distEntry.y < 0.0f)
		{
			normalOut->x = 0.0f;
			normalOut->y = 1.0f;
			normalOut->z = 0.0f;
		}
		else
		{
			normalOut->x = 0.0f;
			normalOut->y = -1.0f;
			normalOut->z = 0.0f;
		}
	}
	else if (entry.z > entry.x && entry.z > entry.y)
	{
		if (distEntry.z < 0.0f)
		{
			normalOut->x = 0.0f;
			normalOut->y = 0.0f;
			normalOut->z = 1.0f;
		}
		else
		{
			normalOut->x = 0.0f;
			normalOut->y = 0.0f;
			normalOut->z = -1.0f;
		}
	}
}

void mc::Entity::addAcceleration(const cl::Vector3f& acceleration)
{
	accel += acceleration;
}

void mc::Entity::setColliderSize(const cl::Vector3f& colliderSize)
{
	centerOffset = colliderSize * 0.5f;
	collider.setSize(colliderSize);
}

void mc::Entity::setPosition(const cl::Vector3f& position)
{
	pos = position;
	pos.update();
	collider.setPosition(pos.p - centerOffset);
}

const cl::Vector2f& mc::Entity::getRotation() const
{
	return rot;
}

const mc::Position& mc::Entity::getPosition() const
{
	return pos;
}
