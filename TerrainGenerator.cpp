#include "TerrainGenerator.h"
//#include "Clock.h"
//#include "Debug.h"

#define WATER_LEVEL 64

mc::TerrainGenerator::TerrainGenerator(int seed)
	: noiseGen(seed), biomes(new mc::Biome[(size_t)mc::BiomeType::BiomeCount])
{
    biomes[0].loadFromeFile("biomes/LightForest.biome");
}

mc::TerrainGenerator::~TerrainGenerator()
{
    delete[] biomes;
}

void mc::TerrainGenerator::generate(Chunk& chunk)
{
    for (uint8_t x = 0; x < CHUNK_WIDTH; x++)
    {
        for (uint8_t z = 0; z < CHUNK_WIDTH; z++)
        {
            for (uint16_t y = 0; y < CHUNK_HEIGHT; y++)
            {
                //float density = (noiseGen.getValue(chunk.pos.x + x, y, 0.03f, 5, 1.5f, 0.55f) + noiseGen.getValue(chunk.pos.x + x, chunk.pos.y + z, 0.03f, 5, 0.7f, 0.7f) + noiseGen.getValue(y, chunk.pos.y + z, 0.03f, 5, 0.7f, 0.7f)) * 20.0f + y;
                //float density = (noiseGen.generate(float(chunk.pos.x + x) * 0.025f, float(y) * 0.04f) + noiseGen.generate(float(chunk.pos.x + x) * 0.025f, float(chunk.pos.y + z) * 0.025f) + noiseGen.generate(float(y) * 0.04f, float(chunk.pos.y + z) * 0.025f)) * 20.0f + y * 1.2f;
                if (biomes[0].hasBlockAt({chunk.pos.x + x, y, chunk.pos.y + z}, noiseGen))
                {
                    chunk.setBlockNoChecks(x, y, z, mc::BlockType::Stone);
                }
                else if (y <= WATER_LEVEL)
                {
                    chunk.setBlockNoChecks(x, y, z, mc::BlockType::Water);
                }
            }

            for (uint16_t y = 1; y < CHUNK_HEIGHT; y++)
            {
                if (chunk.getBlock(x, y, z) == mc::BlockType::Stone)
                {
                    if (chunk.getBlock(x, y + 1, z) == mc::BlockType::Air)
                    {
                        if (y < WATER_LEVEL + 2)
                        {
                            chunk.setBlock(x, y, z, biomes[0].getBeachBlock());
                        }
                        else
                        {
                            chunk.setBlock(x, y, z, biomes[0].getTopBlock());

                            if (biomes[0].hasFlora() && rand.intInRange(0, biomes[0].getFloraFrequencyNum()) == 0 && y > WATER_LEVEL + 2)
                            {
                                chunk.setBlock(x, y + 1, z, biomes[0].getFloraBlock(rand));
                            }
                            else if (biomes[0].hasTrees() && rand.intInRange(0, biomes[0].getTreeFrequencyNum()) == 0 && y > WATER_LEVEL + 5 && x > 0 && x < 15 && z > 0 && z < 15)
                            {
                                /*switch (biomes[0].getTree(rand))
                                {
                                case mc::StructureType::OakTree:
                                    makeOakTree(chunk.getIChunkMap(), rand, { chunk.pos.x + x, y + 1, chunk.pos.y + z });
                                }*/
                                makeOakTree(chunk.getIChunkMap(), rand, { chunk.pos.x + x, y + 1, chunk.pos.y + z });
                            }
                        }
                    }
                    else if (chunk.getBlock(x, y + 1, z) == mc::BlockType::Water)
                    {
                        chunk.setBlock(x, y, z, biomes[0].getBeachBlock());
                    }
                    else if (chunk.getBlock(x, y + 2, z) == mc::BlockType::Air || chunk.getBlock(x, y + 3, z) == mc::BlockType::Air)
                    {
                        chunk.setBlock(x, y, z, biomes[0].getSecondBlock());
                    }
                }
            }
            chunk.setBlock(x, 0, z, mc::BlockType::Bedrock);
        }
    }

    /*rand.setSeed((chunk.pos.x ^ chunk.pos.y) << 2 + noiseGen.getSeed());

    for (uint8_t x = 0; x < CHUNK_WIDTH; x++)
    {
        for (uint8_t z = 0; z < CHUNK_WIDTH; z++)
        {
            int maxY = biomes[0].getHeightAt(cl::Vector2f(chunk.pos.x + x, chunk.pos.y + z), noiseGen);//50.0f * noiseGen.getValue((float)(x + chunk.pos.x), (float)(z + chunk.pos.y), 0.01f, 8, 1.5f, 0.6f) + 90.0f;

            int maxHeightNotAir = (maxY > WATER_LEVEL ? maxY : WATER_LEVEL) + 1;
            for (uint16_t y = 0; y < maxHeightNotAir; y++)
            {
                if (y == 0) { chunk.setBlockNoChecks(x, y, z, mc::BlockType::Bedrock); }
                else if (y < maxY - 3) { chunk.setBlockNoChecks(x, y, z, mc::BlockType::Stone); }
                else if (y < maxY) { chunk.setBlockNoChecks(x, y, z, biomes[0].getSecondBlock()); }
                else if (y == maxY)
                {
                    if (y > WATER_LEVEL + 3) { chunk.setBlockNoChecks(x, y, z, biomes[0].getTopBlock()); }
                    else { chunk.setBlockNoChecks(x, y, z, biomes[0].getBeachBlock()); }
                }
                else if (y <= WATER_LEVEL) { chunk.setBlockNoChecks(x, y, z, mc::BlockType::Water); }
            }

            //plants
            if (biomes[0].hasTrees() && rand.intInRange(0, biomes[0].getTreeFrequencyNum()) == 0 && maxY > WATER_LEVEL + 5 && x > 0 && z > 0 && x < 15 && z < 15)
            {
                switch (biomes[0].getTree(rand))
                {
                case mc::StructureType::OakTree:
                    makeOakTree(chunk.getIChunkMap(), rand, { chunk.pos.x + x, maxY + 1, chunk.pos.y + z });
                }
            }
            else if (biomes[0].hasFlora() && rand.intInRange(0, biomes[0].getFloraFrequencyNum()) == 0 && maxY > WATER_LEVEL + 3)
            {
                chunk.setBlock(x, maxY + 1, z, biomes[0].getFloraBlock(rand));
            }
            else if (rand.intInRange(0, 16) == 0 && maxY == WATER_LEVEL)
            {
                int height = rand.intInRange(1, 5);
                for (int i = 0; i < height; i++)
                {
                    chunk.setBlock(x, i + maxY + 1, z, mc::BlockType::SugarCane);
                }
            }
        }
    }*/

    //cl::debug << "generated in " << clock.getElapsedTime() << '\n';
}
