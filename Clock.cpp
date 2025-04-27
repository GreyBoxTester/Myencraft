#include "Clock.h"

cl::Clock::Clock()
{
	lastTime = high_resolution_clock::now();
}

float cl::Clock::restart()
{
	auto deltaTime = high_resolution_clock::now() - lastTime;
	lastTime += deltaTime;
	return (float)duration_cast<milliseconds>(deltaTime).count() / 1000.0f;
}

float cl::Clock::getElapsedTime()
{
	return (float)duration_cast<microseconds>(high_resolution_clock::now() - lastTime).count() * 0.000001f;
}