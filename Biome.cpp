#include "Biome.h"
#include <fstream>

mc::Biome::Biome() {}

mc::Biome::~Biome()
{
    if (flora != nullptr) { delete[] flora; }
    if (trees != nullptr) { delete[] trees; }
}

void mc::Biome::loadFromeFile(const char* path)
{
    std::ifstream biomeFile(path);

    if (biomeFile)
    {
        std::string str;

        while (!biomeFile.eof())
        {
            biomeFile >> str;
            if (str == "offset")
            {
                biomeFile >> offset;
            }
            else if (str == "amplitude")
            {
                biomeFile >> amplitude;
            }
            else if (str == "heightMul")
            {
                biomeFile >> heightMul;
            }
            else if (str == "xMul")
            {
                biomeFile >> xMul;
            }
            else if (str == "yMul")
            {
                biomeFile >> yMul;
            }
            else if (str == "zMul")
            {
                biomeFile >> zMul;
            }
            else if (str == "octaves")
            {
                biomeFile >> octaves;
            }
            else if (str == "octaveMul1")
            {
                biomeFile >> octaveMul1;
            }
            else if (str == "octaveMul2")
            {
                biomeFile >> octaveMul2;
            }
            else if (str == "topBlock")
            {
                int id;
                biomeFile >> id;
                topBlock = (mc::BlockType)id;
            }
            else if (str == "secondBlock")
            {
                int id;
                biomeFile >> id;
                secondBlock = (mc::BlockType)id;
            }
            else if (str == "beachBlock")
            {
                int id;
                biomeFile >> id;
                beachBlock = (mc::BlockType)id;
            }
            else if (str == "flora")
            {
                biomeFile >> floraCount;
                if (floraCount > 0)
                {
                    flora = new mc::BlockType[floraCount];
                    for (unsigned int i = 0; i < floraCount; i++)
                    {
                        unsigned int n;
                        biomeFile >> n;
                        flora[i] = (mc::BlockType)n;
                    }
                }
            }
            else if (str == "floraFrequencyNum")
            {
                biomeFile >> floraFrequencyNum;
            }
            else if (str == "trees")
            {
                biomeFile >> treeCount;
                if (treeCount > 0)
                {
                    trees = new mc::StructureType[treeCount];
                    for (unsigned int i = 0; i < treeCount; i++)
                    {
                        unsigned int n;
                        biomeFile >> n;
                        trees[i] = (mc::StructureType)n;
                    }
                }
            }
            else if (str == "treeFrequencyNum")
            {
                biomeFile >> treeFrequencyNum;
            }
        }
    }

    biomeFile.close();
}

bool mc::Biome::hasBlockAt(const cl::Vector3i& pos, const cl::NoiseGenerator& noiseGen) const
{
    if (octaves > 1)
    {
        return ((noiseGen.getValue(float(pos.x) * xMul, float(pos.y) * yMul, octaves, octaveMul1, octaveMul2) + noiseGen.getValue(float(pos.x) * xMul, float(pos.z) * zMul, octaves, octaveMul1, octaveMul2) + noiseGen.getValue(float(pos.y) * yMul, float(pos.z) * zMul, octaves, octaveMul1, octaveMul2)) * amplitude + pos.y * heightMul) < offset;
    }
    else
    {
        return ((noiseGen.generate(float(pos.x) * xMul, float(pos.y) * yMul) + noiseGen.generate(float(pos.x) * xMul, float(pos.z) * zMul) + noiseGen.generate(float(pos.y) * yMul, float(pos.z) * zMul)) * amplitude + pos.y * heightMul) < offset;
    }
}

/*int mc::Biome::getHeightAt(const cl::Vector2f& pos, const cl::NoiseGenerator& noiseGen) const
{
    return (int)(amplitude * noiseGen.getValue(pos.x, pos.y, coordMul, noiseOctaves, noiseMod1, noiseMod2) + yOffset);
}*/

mc::BlockType mc::Biome::getTopBlock() const
{
    return topBlock;
}

mc::BlockType mc::Biome::getSecondBlock() const
{
    return secondBlock;
}

mc::BlockType mc::Biome::getBeachBlock() const
{
    return beachBlock;
}

mc::BlockType mc::Biome::getFloraBlock(cl::Random& rand) const
{
    return flora[rand.intInRange(0, floraCount - 1)];
}

int mc::Biome::getFloraFrequencyNum() const
{
    return floraFrequencyNum;
}

bool mc::Biome::hasFlora() const
{
    return floraCount > 0;
}

mc::StructureType mc::Biome::getTree(cl::Random& rand) const
{
    return trees[rand.intInRange(0, treeCount - 1)];
}

int mc::Biome::getTreeFrequencyNum() const
{
    return treeFrequencyNum;
}

bool mc::Biome::hasTrees() const
{
    return treeCount > 0;
}
