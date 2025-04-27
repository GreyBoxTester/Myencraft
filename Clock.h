#pragma once
#include <chrono>

using namespace std::chrono;
namespace cl
{
	class Clock
	{
	public:
		Clock();
		float restart();
		float getElapsedTime();
	private:
		time_point<high_resolution_clock> lastTime;
	};
}
