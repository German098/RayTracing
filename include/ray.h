#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray
{
private:
	// Ray origin
	vec3 origin;
	// Ray direction (unit vector)
	vec3 direction;

public:
	// Constructors
	Ray() { }
	Ray(const point3d& org, const vec3& dir) : origin(org), direction(dir) {};

	// Getters
	const point3d& Origin() const { return origin; }
	const vec3& Direction() const { return direction; }

	// Ray as function P(t) = A + t * b. 
	// P = 3D position along a line 
	// A = origin point at line/ray
	// b = ray direction (unit vector)
	// t = value to move the origin A along the ray/line
	// Return the new origin ray
	point3d at(const double& t) const { return origin + t * direction; }
};

#endif