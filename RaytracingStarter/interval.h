#pragma once

#include "rtstarterutils.h"

class interval
{
public:
	double min, max;

	interval() : min(+rt_infinity), max(-rt_infinity) {} // Default interval is empty

	interval(double min, double max) : min(min), max(max) {}

	double size() const
	{
		return max - min;
	}

	bool contains(double x) const
	{
		return min <= x && max >= x;
	}

	bool surrounds(double x) const
	{
		return min < x && max > x;
	}

	double clamp(double x) const
	{
		if (x < min)
		{
			return min;
		}
		if (x > max)
		{
			return max;
		}
		return x;
	}

	static const interval empty, universe;
};
