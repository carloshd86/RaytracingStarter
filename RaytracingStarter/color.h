#pragma once

#include "vec3.h"
#include "interval.h"

#include <iostream>

using color = vec3;

const color COLOR_WHITE = color(1.0, 1.0, 1.0);
const color COLOR_BLACK = color(0.0, 0.0, 0.0);
const color COLOR_RED = color(1.0, 0.0, 0.0);
const color COLOR_GREEN = color(0.0, 1.0, 0.0);
const color COLOR_BLUE = color(0.0, 0.0, 1.0);

inline double linear_to_gamma(double linear_component)
{
	if (linear_component > 0)
	{
		return std::sqrt(linear_component);
	}

	return 0;
}

inline void write_color(std::ostream& out, const color& pixel_color)
{
	double r = pixel_color.x();
	double g = pixel_color.y();
	double b = pixel_color.z();

	// Apply a linear to gamma transform for gamma 2
	r = linear_to_gamma(r);
	g = linear_to_gamma(g);
	b = linear_to_gamma(b);

	// Translate the [0,1] component values to the byte range [0,255].
	static const interval intensity(0.000, 0.999);
	int rbyte = int(256.0 * intensity.clamp(r));
	int gbyte = int(256.0 * intensity.clamp(g));
	int bbyte = int(256.0 * intensity.clamp(b));

	// Write out the pixel color components.
	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}