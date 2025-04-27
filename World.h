#pragma once
#include "Chunk.h"
#include "TerrainGenerator.h"
#include "Position.h"
#include <unordered_map>
#include <thread>
#include <mutex>

namespace std
{
	template<>
	struct hash<cl::Vector2i>
	{
		size_t operator()(const cl::Vector2i& vec) const
		{
			return (((size_t)vec.x) << 32) ^ (size_t)vec.y;
		}
	};
}

namespace mc
{
	class World : IChunkMap
	{
		friend class RenderCamera;
	public:
		World(cl::Graphics& gfx, int seed, const Position& playerPosition);
		~World();
		BlockType getBlock(const cl::Vector3i& pos) const override;
		void setBlock(const cl::Vector3i& pos, BlockType type) override;
		void setBlockNoChecks(const cl::Vector3i& pos, BlockType type) override;
		void updateMeshes(cl::Graphics& gfx);
		Chunk* getChunk(const cl::Vector2i& chunkPos);
	private:
		std::unordered_map<cl::Vector2i, Chunk> chunks;
		std::mutex* chunkMapMutex;
		std::thread* chunkLoader;
		TerrainGenerator terrainGen;
		bool chunkLoaderThreadTrigger = true;
	};
}
