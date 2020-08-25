#include "DTimer.h"

using namespace std::chrono;

DTimer::DTimer() noexcept
{
	last = steady_clock::now();
}

float DTimer::Delta() noexcept
{
	const auto old = last;
	last = steady_clock::now();
	return duration<float>(last - old).count();
}

float DTimer::Peek() const noexcept
{
	return duration<float>(steady_clock::now() - last).count();
}