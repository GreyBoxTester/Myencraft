#pragma once
#include <random>

namespace cl
{
	class Random
	{
	public:
		Random(int seed = 0);
		void setSeed(int seed);
		int intInRange(int start, int end);
	private:
		std::mt19937 engine;
	};
}
