#pragma once
#include <random>

static size_t RandomRange(size_t min = 10, size_t max = 50)
{
	return rand() % (max - min + 1) + min;
}
