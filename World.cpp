#include "World.h"
//#include "Debug.h"
#include "Clock.h"

#define RENDER_DISTANCE 8
#define UNLOAD_DISTANCE 8

mc::World::World(cl::Graphics& gfx, int seed, const mc::Position& playerPosition)
	: terrainGen(seed), chunkMapMutex(new std::mutex)
{
	mc::Chunk::world = this;

	chunkLoader = new std::thread([](cl::Graphics* gfx, bool* trigger, std::unordered_map<cl::Vector2i, mc::Chunk>* chunks, std::mutex* cmMutex, mc::TerrainGenerator* terrainGen, const mc::Position* playerPos)
	{
		while (*trigger)
		{
			cl::Vector2i chunkPos(playerPos->getChunkPos().x >> 4, playerPos->getChunkPos().y >> 4);

			for (auto& c : *chunks)
			{
				if (c.first.x < chunkPos.x - UNLOAD_DISTANCE || c.first.x > chunkPos.x + UNLOAD_DISTANCE || c.first.y < chunkPos.y - UNLOAD_DISTANCE || c.first.y > chunkPos.y + UNLOAD_DISTANCE)
				{
					cmMutex->lock();
					chunks->erase(c.first);
					cmMutex->unlock();
				}
			}

			for (int x = -RENDER_DISTANCE + chunkPos.x; x < RENDER_DISTANCE + chunkPos.x + 1; x++)
			{
				for (int y = -RENDER_DISTANCE + chunkPos.y; y < RENDER_DISTANCE + chunkPos.y + 1; y++)
				{
					if (!chunks->contains({ x, y }))
					{
						cmMutex->lock();
						Chunk& ch = (*chunks)[{ x, y }];
						ch.init({ x, y });
						terrainGen->generate(ch);
						cmMutex->unlock();
					}
				}
			}

			for (auto& c : *chunks)
			{
				if (!c.second.loaded) 
				{ 
					c.second.loaded = true;
					if (chunks->contains(c.first + cl::Vector2i(0, 1))) 
					{ 
						mc::Chunk& ch = chunks->at(c.first + cl::Vector2i(0, 1));
						ch.meshUpdated = false;
						ch.shouldntDrawLayer.reset();
					}
					if (chunks->contains(c.first + cl::Vector2i(0, -1))) 
					{ 
						mc::Chunk& ch = chunks->at(c.first + cl::Vector2i(0, -1));
						ch.meshUpdated = false;
						ch.shouldntDrawLayer.reset();
					}
					if (chunks->contains(c.first + cl::Vector2i(1, 0)))
					{
						mc::Chunk& ch = chunks->at(c.first + cl::Vector2i(1, 0));
						ch.meshUpdated = false;
						ch.shouldntDrawLayer.reset();
					}
					if (chunks->contains(c.first + cl::Vector2i(-1, 0)))
					{
						mc::Chunk& ch = chunks->at(c.first + cl::Vector2i(-1, 0));
						ch.meshUpdated = false;
						ch.shouldntDrawLayer.reset();
					}
				}
			}

			for (auto& c : *chunks)
			{
				cmMutex->lock();
				if (!c.second.meshUpdated) { c.second.makeMesh(*gfx); }
				cmMutex->unlock();
			}

			std::this_thread::sleep_for(800ms);
		}


	}, &gfx, &chunkLoaderThreadTrigger, &chunks, chunkMapMutex, &terrainGen, &playerPosition);

	//std::this_thread::sleep_for(1s);
}

mc::World::~World()
{
	chunkLoaderThreadTrigger = false;
	chunkLoader->join();
	delete chunkLoader;
	delete chunkMapMutex;
}

mc::BlockType mc::World::getBlock(const cl::Vector3i& pos) const
{
	if (pos.y > 255 || pos.y < 0) { return mc::BlockType::Air; }
	cl::Vector2i cp = CHUNK_POS(pos);
	if (!chunks.contains(cp)) { return mc::BlockType::Grass; }
	return chunks.at(cp).getBlock(pos - CHUNK_TO_3D_BLOCK_SPACE(cp));
}

void mc::World::setBlock(const cl::Vector3i& pos, mc::BlockType type)
{
	if (pos.y > 255 || pos.y < 0) { return; }
	chunkMapMutex->lock();
	cl::Vector2i cp = CHUNK_POS(pos);
	if (!chunks.contains(cp)) { chunkMapMutex->unlock(); return; }
	chunks.at(cp).setBlock(pos - CHUNK_TO_3D_BLOCK_SPACE(cp), type);

	if ((pos.x & (~0xf)) == pos.x && chunks.contains(cp + cl::Vector2i(-1, 0)))
	{
		Chunk& c = chunks.at(cp + cl::Vector2i(-1, 0));
		c.setDrawLayer(pos.y);
		c.meshUpdated = false;
	}
	if ((pos.z & (~0xf)) == pos.z && chunks.contains(cp + cl::Vector2i(0, -1)))
	{
		Chunk& c = chunks.at(cp + cl::Vector2i(0, -1));
		c.setDrawLayer(pos.y);
		c.meshUpdated = false;
	}
	if ((pos.x & (~0xf)) == pos.x - 15 && chunks.contains(cp + cl::Vector2i(1, 0)))
	{
		Chunk& c = chunks.at(cp + cl::Vector2i(1, 0));
		c.setDrawLayer(pos.y);
		c.meshUpdated = false;
	}
	if ((pos.z & (~0xf)) == pos.z - 15 && chunks.contains(cp + cl::Vector2i(0, 1)))
	{
		Chunk& c = chunks.at(cp + cl::Vector2i(0, 1));
		c.setDrawLayer(pos.y);
		c.meshUpdated = false;
	}

	chunkMapMutex->unlock();
}

void mc::World::setBlockNoChecks(const cl::Vector3i& pos, BlockType type)
{
	if (pos.y > 255 || pos.y < 0) { return; }
	cl::Vector2i cp = CHUNK_POS(pos);
	if (!chunks.contains(cp)) { return; }
	chunks.at(cp).setBlock(pos - CHUNK_TO_3D_BLOCK_SPACE(cp), type);

	if ((pos.x & (~0xf)) == pos.x && chunks.contains(cp + cl::Vector2i(-1, 0)))
	{
		Chunk& c = chunks.at(cp + cl::Vector2i(-1, 0));
		c.setDrawLayer(pos.y);
		c.meshUpdated = false;
	}
	if ((pos.z & (~0xf)) == pos.z && chunks.contains(cp + cl::Vector2i(0, -1)))
	{
		Chunk& c = chunks.at(cp + cl::Vector2i(0, -1));
		c.setDrawLayer(pos.y);
		c.meshUpdated = false;
	}
	if ((pos.x & (~0xf)) == pos.x - 15 && chunks.contains(cp + cl::Vector2i(1, 0)))
	{
		Chunk& c = chunks.at(cp + cl::Vector2i(1, 0));
		c.setDrawLayer(pos.y);
		c.meshUpdated = false;
	}
	if ((pos.z & (~0xf)) == pos.z - 15 && chunks.contains(cp + cl::Vector2i(0, 1)))
	{
		Chunk& c = chunks.at(cp + cl::Vector2i(0, 1));
		c.setDrawLayer(pos.y);
		c.meshUpdated = false;
	}
}

void mc::World::updateMeshes(cl::Graphics& gfx)
{
	for (auto& c : chunks)
	{
		chunkMapMutex->lock();
		if (!c.second.meshUpdated) { c.second.makeMesh(gfx); }
		chunkMapMutex->unlock();
	}
}

mc::Chunk* mc::World::getChunk(const cl::Vector2i& chunkPos)
{
	if (!chunks.contains(chunkPos)) { return nullptr; }
	return &chunks.at(chunkPos);
}
