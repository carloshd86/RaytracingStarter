#pragma once

#include <limits>
#include <cmath>
#include <cstdlib>

// Constants

const double rt_infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

inline double random_double() {
	// Returns a random real in [0,1).
	return std::rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
	// Returns a random real in [min,max).
	return min + (max - min) * random_double();
}

inline bool is_nearly_equal(double value, double other)
{
	// Return true if the vector is close to zero in all dimensions.
	constexpr double s = 1.0e-8;
	return (std::fabs(value - other) < s);
}