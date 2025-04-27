#pragma once
#include "Chunk.h"
#include "NoiseGenerator.h"
#include "Random.h"
#include "Biome.h"

namespace mc
{
	class TerrainGenerator
	{
	public:
		TerrainGenerator(int seed);
		~TerrainGenerator();
		void generate(Chunk& chunk);
	private:
		Biome* biomes;
		cl::NoiseGenerator noiseGen;
		cl::Random rand;
	};
}
