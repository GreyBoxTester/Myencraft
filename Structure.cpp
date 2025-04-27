#include "Structure.h"

void mc::makeOakTree(mc::IChunkMap* world, cl::Random& rand, const cl::Vector3i& pos)
{
    //world->setBlock(pos, mc::BlockType::OakLog);
    int height = rand.intInRange(3, 5);
    for (int i = 0; i < height; i++)
    {
        world->setBlockNoChecks({ pos.x, pos.y + i, pos.z }, mc::BlockType::OakLog);
    }

    for (int x = pos.x - 1; x < pos.x + 2; x++)
    {
        for (int y = pos.y + height - 1; y < pos.y + height + 3; y++)
        {
            for (int z = pos.z - 1; z < pos.z + 2; z++)
            {
                if (world->getBlock({ x, y, z }) == mc::BlockType::Air)
                {
                    world->setBlockNoChecks({ x, y, z }, mc::BlockType::OakLeaves);
                }
            }
        }
    }
}