#include "Chunk.h"
#include "NoiseGenerator.h"
//#include "Debug.h"
//#include "Clock.h"

mc::IChunkMap* mc::Chunk::world = nullptr;

mc::Chunk::Chunk()
    : shouldntDrawLayer(0)
{}

void mc::Chunk::init(const cl::Vector2i& chunkPos)
{
    pos = CHUNK_TO_BLOCK_SPACE(chunkPos);
    blocks = new mc::BlockType[CHUNK_BLOCKS_COUNT];
    memset(blocks, 0u, CHUNK_BLOCKS_COUNT);
}

mc::Chunk::~Chunk()
{
    if (blocks != nullptr) { delete[] blocks; }
    freeMesh();
}

mc::BlockType mc::Chunk::getBlock(int x, int y, int z) const
{
    if (y < 0 || y > 255) { return mc::BlockType::Air; }
    if (x < 0 || x > 15 || z < 0 || z > 15) { return world->getBlock(cl::Vector3i(x + pos.x, y, z + pos.y)); }
    return blocks[y * CHUNK_W_BY_W + x * CHUNK_WIDTH + z];
}

void mc::Chunk::setBlock(int x, int y, int z, mc::BlockType type)
{
    if (x < 0 || x > 15 || y < 0 || y > 255 || z < 0 || z > 15) { return; }
    unsigned int ind = y * CHUNK_W_BY_W + x * CHUNK_WIDTH + z;
    mc::BlockType prev = blocks[ind];
    blocks[ind] = type;
    if ((IS_FULL_BLOCK(type) && !IS_FULL_BLOCK(prev)) || (!IS_FULL_BLOCK(type) && IS_FULL_BLOCK(prev))) { setDrawLayer(y); meshUpdated = false; }
    //if (type != prev) { updated = false; }
} 

mc::BlockType mc::Chunk::getBlock(const cl::Vector3i& blockPos) const
{
    if (blockPos.y < 0 || blockPos.y > 255) { return mc::BlockType::Air; }
    if (blockPos.x < 0 || blockPos.x > 15 || blockPos.z < 0 || blockPos.z > 15) { return world->getBlock(cl::Vector3i(blockPos.x + pos.x, blockPos.y, blockPos.z + pos.y)); }
    return blocks[blockPos.y * CHUNK_W_BY_W + blockPos.x * CHUNK_WIDTH + blockPos.z];
}

void mc::Chunk::setBlock(const cl::Vector3i& blockPos, mc::BlockType type)
{
    if (blockPos.x < 0 || blockPos.x > 15 || blockPos.y < 0 || blockPos.y > 255 || blockPos.z < 0 || blockPos.z > 15) { return; }
    unsigned int ind = blockPos.y * CHUNK_W_BY_W + blockPos.x * CHUNK_WIDTH + blockPos.z;
    mc::BlockType prev = blocks[ind];
    blocks[ind] = type;
    if (!(IS_FULL_BLOCK(type) && IS_FULL_BLOCK(prev)) || !(IS_DRAWABLE_BLOCK(type) && IS_DRAWABLE_BLOCK(prev))) { setDrawLayer(blockPos.y); meshUpdated = false; }
    //if (type != prev) {  }
}

void addIndices(std::vector<unsigned int>* ind, unsigned int startInd)
{
    ind->push_back(startInd);
    ind->push_back(startInd + 1);
    ind->push_back(startInd + 2);
    ind->push_back(startInd + 2);
    ind->push_back(startInd + 1);
    ind->push_back(startInd + 3);
}

void addIndicesRev(std::vector<unsigned int>* ind, unsigned int startInd)
{
    ind->push_back(startInd + 3);
    ind->push_back(startInd + 1);
    ind->push_back(startInd + 2);
    ind->push_back(startInd + 2);
    ind->push_back(startInd + 1);
    ind->push_back(startInd);
}

void mc::Chunk::makeMesh(cl::Graphics& gfx)
{
    freeMesh();

    cl::IndexedTriangleList<mc::Vertex> mesh;
    mesh.vertices.reserve(CHUNK_BLOCKS_COUNT * 4);
    mesh.vertices.reserve(CHUNK_BLOCKS_COUNT * 6);

    cl::IndexedTriangleList<mc::Vertex> plantsMesh;
    plantsMesh.vertices.reserve(100);
    plantsMesh.indices.reserve(150);

    cl::IndexedTriangleList<mc::Vertex> waterMesh;
    waterMesh.vertices.reserve(100);
    waterMesh.indices.reserve(150);

    for (uint16_t y = 0; y < CHUNK_HEIGHT; y++)
    {
        if (shouldntDrawLayer[y])
        {
            continue;
        }

        bool layerHasNoFaces = true;

        for (uint8_t x = 0; x < CHUNK_WIDTH; x++)
        {
            for (uint8_t z = 0; z < CHUNK_WIDTH; z++)
            {
                mc::BlockType block = getBlock(x, y, z);

                if (IS_DRAWABLE_BLOCK(block))
                {
                    cl::Vector3f blockPoint(x, y, z);

                    if (IS_X_SHAPED(block))
                    {
                        if (IS_TRANSPARENT_BLOCK(getBlock(x, y, z - 1)) || IS_TRANSPARENT_BLOCK(getBlock(x, y, z + 1)) ||
                            IS_TRANSPARENT_BLOCK(getBlock(x - 1, y, z)) || IS_TRANSPARENT_BLOCK(getBlock(x + 1, y, z)) ||
                            IS_TRANSPARENT_BLOCK(getBlock(x, y - 1, z)) || IS_TRANSPARENT_BLOCK(getBlock(x, y + 1, z))
                            )
                        {
                            layerHasNoFaces = false;

                            addIndices(&plantsMesh.indices, plantsMesh.vertices.size());

                            plantsMesh.vertices.push_back({ blockPoint,                       {}, { textureData[(int)block].side.x,                    textureData[(int)block].side.y + BLOCK_PIXEL_SIZE } });
                            plantsMesh.vertices.push_back({ blockPoint + cl::Vector3f(0,1,0), {}, { textureData[(int)block].side.x,                    textureData[(int)block].side.y                    } });
                            plantsMesh.vertices.push_back({ blockPoint + cl::Vector3f(1,0,1), {}, { textureData[(int)block].side.x + BLOCK_PIXEL_SIZE, textureData[(int)block].side.y + BLOCK_PIXEL_SIZE } });
                            plantsMesh.vertices.push_back({ blockPoint + cl::Vector3f(1,1,1), {}, { textureData[(int)block].side.x + BLOCK_PIXEL_SIZE, textureData[(int)block].side.y                    } });

                            addIndices(&plantsMesh.indices, plantsMesh.vertices.size());

                            plantsMesh.vertices.push_back({ blockPoint + cl::Vector3f(0,0,1), {}, { textureData[(int)block].side.x,                    textureData[(int)block].side.y + BLOCK_PIXEL_SIZE } });
                            plantsMesh.vertices.push_back({ blockPoint + cl::Vector3f(0,1,1), {}, { textureData[(int)block].side.x,                    textureData[(int)block].side.y                    } });
                            plantsMesh.vertices.push_back({ blockPoint + cl::Vector3f(1,0,0), {}, { textureData[(int)block].side.x + BLOCK_PIXEL_SIZE, textureData[(int)block].side.y + BLOCK_PIXEL_SIZE } });
                            plantsMesh.vertices.push_back({ blockPoint + cl::Vector3f(1,1,0), {}, { textureData[(int)block].side.x + BLOCK_PIXEL_SIZE, textureData[(int)block].side.y                    } });
                            
                            addIndicesRev(&plantsMesh.indices, plantsMesh.vertices.size());

                            plantsMesh.vertices.push_back({ blockPoint,                       {}, { textureData[(int)block].side.x,                    textureData[(int)block].side.y + BLOCK_PIXEL_SIZE } });
                            plantsMesh.vertices.push_back({ blockPoint + cl::Vector3f(0,1,0), {}, { textureData[(int)block].side.x,                    textureData[(int)block].side.y                    } });
                            plantsMesh.vertices.push_back({ blockPoint + cl::Vector3f(1,0,1), {}, { textureData[(int)block].side.x + BLOCK_PIXEL_SIZE, textureData[(int)block].side.y + BLOCK_PIXEL_SIZE } });
                            plantsMesh.vertices.push_back({ blockPoint + cl::Vector3f(1,1,1), {}, { textureData[(int)block].side.x + BLOCK_PIXEL_SIZE, textureData[(int)block].side.y                    } });

                            addIndicesRev(&plantsMesh.indices, plantsMesh.vertices.size());

                            plantsMesh.vertices.push_back({ blockPoint + cl::Vector3f(0,0,1), {}, { textureData[(int)block].side.x,                    textureData[(int)block].side.y + BLOCK_PIXEL_SIZE } });
                            plantsMesh.vertices.push_back({ blockPoint + cl::Vector3f(0,1,1), {}, { textureData[(int)block].side.x,                    textureData[(int)block].side.y                    } });
                            plantsMesh.vertices.push_back({ blockPoint + cl::Vector3f(1,0,0), {}, { textureData[(int)block].side.x + BLOCK_PIXEL_SIZE, textureData[(int)block].side.y + BLOCK_PIXEL_SIZE } });
                            plantsMesh.vertices.push_back({ blockPoint + cl::Vector3f(1,1,0), {}, { textureData[(int)block].side.x + BLOCK_PIXEL_SIZE, textureData[(int)block].side.y                    } });

                        }
                    }
                    else
                    {
                        cl::IndexedTriangleList<mc::Vertex>& currentMesh = (IS_PLANT(block) ? plantsMesh : (IS_FLUID(block) ? waterMesh : mesh));

                        BlockType neighbour;
                        //front
                        neighbour = getBlock(x, y, z - 1);
                        if (IS_TRANSPARENT_BLOCK(neighbour) && neighbour != block /*|| block == BlockType::OakLeaves*/)
                        {
                            layerHasNoFaces = false;

                            addIndices(&currentMesh.indices, currentMesh.vertices.size());

                            currentMesh.vertices.push_back({ blockPoint,                       {0.0f,0.0f,-1.0f}, { textureData[(int)block].side.x,                    textureData[(int)block].side.y + BLOCK_PIXEL_SIZE } });
                            currentMesh.vertices.push_back({ blockPoint + cl::Vector3f(0,1,0), {0.0f,0.0f,-1.0f}, { textureData[(int)block].side.x,                    textureData[(int)block].side.y                    } });
                            currentMesh.vertices.push_back({ blockPoint + cl::Vector3f(1,0,0), {0.0f,0.0f,-1.0f}, { textureData[(int)block].side.x + BLOCK_PIXEL_SIZE, textureData[(int)block].side.y + BLOCK_PIXEL_SIZE } });
                            currentMesh.vertices.push_back({ blockPoint + cl::Vector3f(1,1,0), {0.0f,0.0f,-1.0f}, { textureData[(int)block].side.x + BLOCK_PIXEL_SIZE, textureData[(int)block].side.y                    } });
                        }

                        //back
                        neighbour = getBlock(x, y, z + 1);
                        if (IS_TRANSPARENT_BLOCK(neighbour) && neighbour != block /*|| block == BlockType::OakLeaves*/)
                        {
                            layerHasNoFaces = false;

                            addIndices(&currentMesh.indices, currentMesh.vertices.size());

                            currentMesh.vertices.push_back({ blockPoint + cl::Vector3f(1,0,1), {0.0f,0.0f,1.0f}, { textureData[(int)block].side.x,                    textureData[(int)block].side.y + BLOCK_PIXEL_SIZE } });
                            currentMesh.vertices.push_back({ blockPoint + cl::Vector3f(1,1,1), {0.0f,0.0f,1.0f}, { textureData[(int)block].side.x,                    textureData[(int)block].side.y                    } });
                            currentMesh.vertices.push_back({ blockPoint + cl::Vector3f(0,0,1), {0.0f,0.0f,1.0f}, { textureData[(int)block].side.x + BLOCK_PIXEL_SIZE, textureData[(int)block].side.y + BLOCK_PIXEL_SIZE } });
                            currentMesh.vertices.push_back({ blockPoint + cl::Vector3f(0,1,1), {0.0f,0.0f,1.0f}, { textureData[(int)block].side.x + BLOCK_PIXEL_SIZE, textureData[(int)block].side.y                    } });
                        }

                        //left
                        neighbour = getBlock(x - 1, y, z);
                        if (IS_TRANSPARENT_BLOCK(neighbour) && neighbour != block /*|| block == BlockType::OakLeaves*/)
                        {
                            layerHasNoFaces = false;

                            addIndices(&currentMesh.indices, currentMesh.vertices.size());

                            currentMesh.vertices.push_back({ blockPoint + cl::Vector3f(0,0,1), {-1.0f,0.0f,0.0f}, { textureData[(int)block].side.x,                    textureData[(int)block].side.y + BLOCK_PIXEL_SIZE } });
                            currentMesh.vertices.push_back({ blockPoint + cl::Vector3f(0,1,1), {-1.0f,0.0f,0.0f}, { textureData[(int)block].side.x,                    textureData[(int)block].side.y                    } });
                            currentMesh.vertices.push_back({ blockPoint,                       {-1.0f,0.0f,0.0f}, { textureData[(int)block].side.x + BLOCK_PIXEL_SIZE, textureData[(int)block].side.y + BLOCK_PIXEL_SIZE } });
                            currentMesh.vertices.push_back({ blockPoint + cl::Vector3f(0,1,0), {-1.0f,0.0f,0.0f}, { textureData[(int)block].side.x + BLOCK_PIXEL_SIZE, textureData[(int)block].side.y                    } });
                        }

                        //right
                        neighbour = getBlock(x + 1, y, z);
                        if (IS_TRANSPARENT_BLOCK(neighbour) && neighbour != block /*|| block == BlockType::OakLeaves*/)
                        {
                            layerHasNoFaces = false;

                            addIndices(&currentMesh.indices, currentMesh.vertices.size());

                            currentMesh.vertices.push_back({ blockPoint + cl::Vector3f(1,0,0), {1.0f,0.0f,0.0f}, { textureData[(int)block].side.x,                    textureData[(int)block].side.y + BLOCK_PIXEL_SIZE } });
                            currentMesh.vertices.push_back({ blockPoint + cl::Vector3f(1,1,0), {1.0f,0.0f,0.0f}, { textureData[(int)block].side.x,                    textureData[(int)block].side.y                    } });
                            currentMesh.vertices.push_back({ blockPoint + cl::Vector3f(1,0,1), {1.0f,0.0f,0.0f}, { textureData[(int)block].side.x + BLOCK_PIXEL_SIZE, textureData[(int)block].side.y + BLOCK_PIXEL_SIZE } });
                            currentMesh.vertices.push_back({ blockPoint + cl::Vector3f(1,1,1), {1.0f,0.0f,0.0f}, { textureData[(int)block].side.x + BLOCK_PIXEL_SIZE, textureData[(int)block].side.y                    } });
                        }

                        //top
                        neighbour = getBlock(x, y + 1, z);
                        if (IS_TRANSPARENT_BLOCK(neighbour) && neighbour != block /*|| block == BlockType::OakLeaves*/)
                        {
                            layerHasNoFaces = false;

                            addIndices(&currentMesh.indices, currentMesh.vertices.size());

                            currentMesh.vertices.push_back({ blockPoint + cl::Vector3f(0,1,0), {0.0f,1.0f,0.0f}, { textureData[(int)block].top.x,                    textureData[(int)block].top.y + BLOCK_PIXEL_SIZE } });
                            currentMesh.vertices.push_back({ blockPoint + cl::Vector3f(0,1,1), {0.0f,1.0f,0.0f}, { textureData[(int)block].top.x,                    textureData[(int)block].top.y                    } });
                            currentMesh.vertices.push_back({ blockPoint + cl::Vector3f(1,1,0), {0.0f,1.0f,0.0f}, { textureData[(int)block].top.x + BLOCK_PIXEL_SIZE, textureData[(int)block].top.y + BLOCK_PIXEL_SIZE } });
                            currentMesh.vertices.push_back({ blockPoint + cl::Vector3f(1,1,1), {0.0f,1.0f,0.0f}, { textureData[(int)block].top.x + BLOCK_PIXEL_SIZE, textureData[(int)block].top.y                    } });
                        }

                        //bottom
                        neighbour = getBlock(x, y - 1, z);
                        if (IS_TRANSPARENT_BLOCK(neighbour) && neighbour != block /*|| block == BlockType::OakLeaves*/)
                        {
                            layerHasNoFaces = false;

                            addIndices(&currentMesh.indices, currentMesh.vertices.size());

                            currentMesh.vertices.push_back({ blockPoint + cl::Vector3f(0,0,1), {0.0f,-1.0f,0.0f}, { textureData[(int)block].bottom.x,                    textureData[(int)block].bottom.y + BLOCK_PIXEL_SIZE } });
                            currentMesh.vertices.push_back({ blockPoint,                       {0.0f,-1.0f,0.0f}, { textureData[(int)block].bottom.x,                    textureData[(int)block].bottom.y                    } });
                            currentMesh.vertices.push_back({ blockPoint + cl::Vector3f(1,0,1), {0.0f,-1.0f,0.0f}, { textureData[(int)block].bottom.x + BLOCK_PIXEL_SIZE, textureData[(int)block].bottom.y + BLOCK_PIXEL_SIZE } });
                            currentMesh.vertices.push_back({ blockPoint + cl::Vector3f(1,0,0), {0.0f,-1.0f,0.0f}, { textureData[(int)block].bottom.x + BLOCK_PIXEL_SIZE, textureData[(int)block].bottom.y                    } });
                        }
                    }
                }
            }
        }

        if (layerHasNoFaces)
        {
            shouldntDrawLayer[y] = true;
        }
    }

    plantsMesh.calculateNormals();

    if (mesh.vertices.size() != 0)
    {
        vertexBuffer = new cl::VertexBuffer(gfx, mesh.vertices.data(), mesh.vertices.size());
        indexBuffer = new cl::IndexBuffer(gfx, mesh.indices.data(), mesh.indices.size());
    }

    if (plantsMesh.vertices.size() != 0)
    {
        plantsVB = new cl::VertexBuffer(gfx, plantsMesh.vertices.data(), plantsMesh.vertices.size());
        plantsIB = new cl::IndexBuffer(gfx, plantsMesh.indices.data(), plantsMesh.indices.size());
    }

    if (waterMesh.vertices.size() != 0)
    {
        waterVB = new cl::VertexBuffer(gfx, waterMesh.vertices.data(), waterMesh.vertices.size());
        waterIB = new cl::IndexBuffer(gfx, waterMesh.indices.data(), waterMesh.indices.size());
    }

    meshUpdated = true;

    //cl::debug << clock.getElapsedTime() << '\n';
}

void mc::Chunk::freeMesh()
{
    if (vertexBuffer != nullptr) 
    { 
        delete vertexBuffer; 
        vertexBuffer = nullptr; 
    }
    if (indexBuffer != nullptr) 
    { 
        delete indexBuffer; 
        indexBuffer = nullptr; 
    }
    if (plantsVB != nullptr) 
    { 
        delete plantsVB; 
        plantsVB = nullptr; 
    }
    if (plantsIB != nullptr) 
    { 
        delete plantsIB; 
        plantsIB = nullptr; 
    }
    if (waterVB != nullptr) 
    { 
        delete waterVB; 
        waterVB = nullptr; 
    }
    if (waterIB != nullptr) 
    { 
        delete waterIB; 
        waterIB = nullptr; 
    }
} 

bool mc::Chunk::isMeshUpdated()
{
    return meshUpdated;
}

const cl::Vector2i& mc::Chunk::getPos() const
{
    return pos;
}

void mc::Chunk::getBounds(AABB* aabbOut) const
{
    aabbOut->setPosition(cl::Vector3f(pos.x, 0.0f, pos.y));
    aabbOut->setSize(cl::Vector3f(CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_WIDTH));
}

void mc::Chunk::getBounds(AABB* aabbOut, const cl::Vector2i& relativeChunkPos) const
{
    aabbOut->setPosition(cl::Vector3f(pos.x - relativeChunkPos.x, 0.0f, pos.y - relativeChunkPos.y));
    aabbOut->setSize(cl::Vector3f(CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_WIDTH));
}

mc::IChunkMap* mc::Chunk::getIChunkMap() const
{
    return world;
}

void mc::Chunk::setDrawLayer(int y)
{
    shouldntDrawLayer[y] = false;
    if (y > 0) { shouldntDrawLayer[y - 1] = false; }
    if (y < 255) { shouldntDrawLayer[y + 1] = false; }
}

void mc::Chunk::setBlockNoChecks(int x, int y, int z, BlockType type)
{
    blocks[y * CHUNK_W_BY_W + x * CHUNK_WIDTH + z] = type;
}
