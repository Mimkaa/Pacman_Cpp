#pragma once
#include <chrono>
class Timer
{
public:
	std::chrono::time_point<std::chrono::steady_clock> StartTime;
	bool exceeded(float seconds);
	void start();
};

