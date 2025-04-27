#pragma once
#include "BlockConstants.h"
#include "NoiseGenerator.h"
#include "Random.h"
#include "Structure.h"

namespace mc
{
	enum class BiomeType : unsigned char
	{
		LightForest = 0u,
		BiomeCount
	};

	class Biome
	{
	public:
		Biome();
		~Biome();
		void loadFromeFile(const char* path);
		bool hasBlockAt(const cl::Vector3i& pos, const cl::NoiseGenerator& noiseGen) const;
		BlockType getTopBlock() const;
		BlockType getSecondBlock() const;
		BlockType getBeachBlock() const;

		BlockType getFloraBlock(cl::Random& rand) const;
		int getFloraFrequencyNum() const;
		bool hasFlora() const;

		StructureType getTree(cl::Random& rand) const;
		int getTreeFrequencyNum() const;
		bool hasTrees() const;
	private:
		BlockType topBlock = BlockType::Grass;
		BlockType secondBlock = BlockType::Dirt;
		BlockType beachBlock = BlockType::Sand;

		//flora
		BlockType* flora = nullptr;
		unsigned int floraCount = 0;
		int floraFrequencyNum = 0;

		//trees
		StructureType* trees = nullptr;
		unsigned int treeCount = 0;
		int treeFrequencyNum = 0;

		//noise
		float offset = 80.0f;
		float amplitude = 20.0f;
		float heightMul = 1.2f;
		float xMul = 0.025f;
		float yMul = 0.04f;
		float zMul = 0.025f;
		int octaves = 3;
		float octaveMul1 = 1.5f;
		float octaveMul2 = 0.6f;
	};

	/*class Biome
	{
	public:
		Biome();
		~Biome();
		void loadFromeFile(const char* path);
		int getHeightAt(const cl::Vector2f& pos, const cl::NoiseGenerator& noiseGen) const;
		mc::BlockType getTopBlock() const;
		mc::BlockType getSecondBlock() const;
		mc::BlockType getBeachBlock() const;

		mc::BlockType getFloraBlock(cl::Random& rand) const;
		int getFloraFrequencyNum() const;
		bool hasFlora() const;

		mc::StructureType getTree(cl::Random& rand) const;
		int getTreeFrequencyNum() const;
		bool hasTrees() const;
	private:
		mc::BlockType topBlock = mc::BlockType::Grass;
		mc::BlockType secondBlock = mc::BlockType::Dirt;
		mc::BlockType beachBlock = mc::BlockType::Sand;

		//flora
		mc::BlockType* flora = nullptr;
		unsigned int floraCount = 0;
		int floraFrequencyNum = 0;

		//trees
		mc::StructureType* trees = nullptr;
		unsigned int treeCount = 0;
		int treeFrequencyNum = 0;

		//noise
		float yOffset = 92.0f;
		float amplitude = 50.0f;
		int noiseOctaves = 8;
		float noiseMod1 = 1.5f;
		float noiseMod2 = 0.6f;
		float coordMul = 0.01f;
	};*/
}
