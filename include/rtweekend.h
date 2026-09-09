#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cstdlib>

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

#endif // !define RTWEEKEND_H
