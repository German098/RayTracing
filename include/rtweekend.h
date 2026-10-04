#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cstdlib>
#include <limits>

// Constants
const double INF = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

// Utility functions (inline to avoid linker errors for multiple function definitions by 
// including rtweekend.h in multiple .cpp files)
inline double RandomDouble()
{
	// Return value in range [0.0, 1.0) (+ 1.0 to transform in double's operation and 
	// avoid return 1.0)
	return std::rand() / (RAND_MAX + 1.0);
}

inline double RandomDouble(const double& min, const double& max)
{
	// Return value in range [min, max]
	return min + (max - min) * RandomDouble();
}

inline double DegreesToRadians(const double& angle)
{
	return angle * PI / 180.0;
}

#endif // !define RTWEEKEND_H
