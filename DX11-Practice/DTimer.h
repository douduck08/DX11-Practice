#pragma once
#include <chrono>

class DTimer
{
public:
	DTimer() noexcept;
	float Delta() noexcept;
	float Peek() const noexcept;
private:
	std::chrono::steady_clock::time_point last;
};