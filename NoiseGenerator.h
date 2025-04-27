#pragma once
#include <math.h>

namespace cl
{
	class NoiseGenerator
	{
	public:
		NoiseGenerator(int seed);
		/*float getValue(float x, float z) const;*/
		float getValue(float x, float z, int octaves, float mod1, float mod2) const;
		float generate(float x, float z) const;
		float findNoise2(float x, float z) const;
		int getSeed() const;
	private:
		float findNoise(int n) const;
		float lerp(float s, float e, float n) const;
	private:
		int seed;
	};
}
