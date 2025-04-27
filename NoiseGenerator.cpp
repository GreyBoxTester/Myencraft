#include "NoiseGenerator.h"

cl::NoiseGenerator::NoiseGenerator(int seed)
    :seed(seed)
{}

/*float cl::NoiseGenerator::getValue(float x, float z) const
{
    float mod = 1.0f;
    float total = 0.0f;
    float divValue = 0.0f;
    for (int i = 0; i < 5; i++)
    {
        total += generate(x, z) * mod;
        divValue += mod;
        x *= 1.5f;
        z *= 1.5f;
        mod *= 0.6f;
    }
    return total / divValue;

    //hilly
    x *= 0.05f;
    z *= 0.05f;
    float mod = 1.0f;
    float total = 0.0f;
    float divValue = 0.0f;
    for (int i = 0; i < 8; i++)
    {
        total += generate(x, z) * mod;
        divValue += mod;
        x *= 0.3f;
        z *= 0.3f;
        mod *= 0.6f;
    }
    return total / divValue;

    x *= 0.01f;
    z *= 0.01f;
    return (generate(x, z) + generate(x * 0.25f, z * 0.25f) * 0.5f + generate(x * 0.1f, z * 0.1f) * 0.25f) / 1.75f;
}*/

float cl::NoiseGenerator::getValue(float x, float z, int octaves, float mod1, float mod2) const
{
    float mod = 1.0f;
    float total = 0.0f;
    float divValue = 0.0f;
    for (int i = 0; i < octaves; i++)
    {
        total += generate(x, z) * mod;
        divValue += mod;
        x *= mod1;
        z *= mod1;
        mod *= mod2;
    }
    return total / divValue;
}

float cl::NoiseGenerator::generate(float x, float z) const
{
    float sx = floorf(x);
    float sz = floorf(z);

    return lerp(
        lerp(findNoise2(sx, sz), findNoise2(sx + 1.0f, sz), x - sx),
        lerp(findNoise2(sx, sz + 1.0f), findNoise2(sx + 1.0f, sz + 1.0f), x - sx),
        z - sz
    );
}

float cl::NoiseGenerator::findNoise(int n) const
{
    n += seed;
    n = (n << 13) ^ n;
    return 1.0f - ((float)((n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff) / 1073741824.0f); //???
}

float cl::NoiseGenerator::findNoise2(float x, float z) const
{
    return findNoise(x + z * 57.0f);
}

int cl::NoiseGenerator::getSeed() const
{
    return seed;
}

float cl::NoiseGenerator::lerp(float s, float e, float n) const
{
    return (n * n * (3 - 2 * n)) * (e - s) + s;
    //return (n * n) * (e - s) + s;
}
