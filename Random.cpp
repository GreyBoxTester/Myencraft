#include "Random.h"

cl::Random::Random(int seed)
    : engine(seed)
{
    for (int i = 0; i < 5; i++)
    {
        intInRange(i, i * 5);
    }
}

void cl::Random::setSeed(int seed)
{
    engine.seed(seed);
}

int cl::Random::intInRange(int start, int end)
{
    return std::uniform_int_distribution<int>(start, end)(engine);
    //return ((n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff) % (end - start + 1) + start; //???
}
