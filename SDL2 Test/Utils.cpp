#include "Utils.h"

int Utils::clamp(int n, int lower, int upper)
{
	if (n < lower)
		return lower;
	if (n > upper)
		return upper;
	return n;
}