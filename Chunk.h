#pragma once
#include "Vertex.h"
#include "IndexedTriangleList.h"
#include "IChunkMap.h"
#include "AABB.h"
#include <bitset>

#define CHUNK_WIDTH 16u
#define CHUNK_W_BY_W 256u
#define CHUNK_HEIGHT 256u
#define CHUNK_BLOCKS_COUNT 65536u

#define CHUNK_POS(blockPos) cl::Vector2i((blockPos).x>>4,(blockPos).z>>4)
#define CHUNK_POS_BLOCK_SPACE(blockPos) cl::Vector2i((blockPos).x&(~0xf),(blockPos).z&(~0xf))

#define CHUNK_TO_BLOCK_SPACE(chunkPos) cl::Vector2i((chunkPos).x<<4,(chunkPos).y<<4)
#define CHUNK_TO_3D_BLOCK_SPACE(chunkPos) cl::Vector3i((chunkPos).x<<4,0,(chunkPos).y<<4)
//#define CHUNK_TO_3D_BLOCK_SPACE_FLOAT(chunkPos) cl::Vector3f((chunkPos).x<<4,0.0f,(chunkPos).y<<4)

namespace mc
{
	class Chunk
	{
		friend class RenderCamera;
		friend class World;
		friend class TerrainGenerator;
	public:
		Chunk();
		~Chunk();
		void init(const cl::Vector2i& chunkPos);
		BlockType getBlock(int x, int y, int z) const;
		void setBlock(int x, int y, int z, BlockType type);
		BlockType getBlock(const cl::Vector3i& blockPos) const;
		void setBlock(const cl::Vector3i& blockPos, BlockType type);
		void makeMesh(cl::Graphics& gfx);
		void freeMesh();
		bool isMeshUpdated();
		const cl::Vector2i& getPos() const;
		void getBounds(AABB* aabbOut) const;
		void getBounds(AABB* aabbOut, const cl::Vector2i& relativeChunkPos) const;
		IChunkMap* getIChunkMap() const;
	private:
		void setDrawLayer(int y);
		void setBlockNoChecks(int x, int y, int z, BlockType type);
	private:
		BlockType* blocks = nullptr;
		cl::Vector2i pos;
		cl::VertexBuffer* vertexBuffer = nullptr;
		cl::IndexBuffer* indexBuffer = nullptr;
		cl::VertexBuffer* plantsVB = nullptr;
		cl::IndexBuffer* plantsIB = nullptr;
		cl::VertexBuffer* waterVB = nullptr;
		cl::IndexBuffer* waterIB = nullptr;
		std::bitset<256> shouldntDrawLayer; //to optimize making mesh
		bool meshUpdated = true;
		bool loaded = false;

		static IChunkMap* world;
	};
}
