#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <math.h>
#include <fstream>
#include <ostream>

#include "rtweekend.h"

class vec3
{
private:
	double e[3];

public:
	// Constructors
	vec3() : e{ 0, 0, 0 } {}
	vec3(const double& e1, const double& e2, const double& e3) : e{ e1, e2, e3 } {}

	// Getters
	double X() const { return e[0]; }
	double Y() const { return e[1]; }
	double Z() const { return e[2]; }

	// Overload operators
	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	double operator[](const int& i) const { return e[i]; }
	double& operator[](const int& i) { return e[i]; }
	vec3& operator+=(const vec3& v)
	{
		e[0] += v[0];
		e[1] += v[1];
		e[2] += v[2];

		return *this;
	}
	vec3& operator*=(const double& a)
	{
		e[0] *= a;
		e[1] *= a;
		e[2] *= a;

		return *this;
	}
	vec3& operator/=(const double& a)
	{
		return *this *= 1 / a;
	}

	// Length
	double LengthSquared() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	// Length
	double Length() const
	{
		return std::sqrt(LengthSquared());
	}

	// Return true, if vec3 is close to zero for all components
	bool NearZero() const 
	{
		const double a = 1e-8;

		return (std::fabs(e[0]) < a && std::fabs(e[1]) < a && std::fabs(e[2]) < a);
	}

	// Generate randoms vec3
	static vec3 Random()
	{
		return vec3(RandomDouble(), RandomDouble(), RandomDouble());
	}

	static vec3 Random(const double& min, const double& max)
	{
		return vec3(RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max));
	}
};

// Alias for vec3 (useful for geometric clarity in the code)
using point3d = vec3;
// Alias for color
using color = vec3;

// Vector utility functions
inline std::ostream& operator<<(std::ofstream& out, const vec3& vec)
{
	out << vec.X() << " " << vec.Y() << " " << vec.Z();

	return out;
}

inline vec3 operator+(const vec3& u, const vec3& v)
{
	return vec3(u.X() + v.X(), u.Y() + v.Y(), u.Z() + v.Z());
}

inline vec3 operator+(const vec3& u, const double& a)
{
	return vec3(u.X() + a, u.Y() + a, u.Z() + a);
}

inline vec3 operator-(const vec3& u, const vec3& v)
{
	return vec3(u.X() - v.X(), u.Y() - v.Y(), u.Z() - v.Z());
}

inline vec3 operator*(const vec3& u, const vec3& v)
{
	return vec3(u.X() * v.X(), u.Y() * v.Y(), u.Z() * v.Z());
}

inline vec3 operator*(const double& a, const vec3& v)
{
	return vec3(v.X() * a, v.Y() * a, v.Z() * a);
}

inline vec3 operator*(const vec3& v, const double& a)
{
	return a * v;
}

inline vec3 operator/(const vec3& v, const double& a)
{
	return v * (1.0 / a);
}

inline double dot(const vec3& a, const vec3& b)
{
	return a.X() * b.X() + a.Y() * b.Y() + a.Z() * b.Z();
}

inline vec3 cross(const vec3& a, const vec3& b)
{
	return vec3(
		a.Y() * b.Z() - a.Z() * b.Y(),
		a.Z() * b.X() - a.X() * b.Z(),
		a.X() * b.Y() - a.Y() * b.X()
	);
}

inline vec3 unit_vector(const vec3& a)
{
	return a / a.Length();
}

// Get reflect ray from incident ray v and normal n. N has to be normalized.
// If v is normalized, return reflect vector will also be nromalized.
inline vec3 Reflect(const vec3& v, const vec3& n)
{
	// First, calculate v prjection over n => b = dot(v, n) (it tells us 
	// how much of v lies in the direction of n). |n| = 1, so dot(v, n) =
	// |v|*cos(theta) = +/- v projection's length over n. We negate the result,
	// becasue we want b to point out of the surface, so: b = -(dot(v, n))*n 
	// (multiply by n to get the projection vector that points at same direction 
	// of n). We need to invert sign of normal component of v to points out of 
	// surface. E.g.: n = (0, 1) and v = (1, -2), -b = (0, -2), so normal component
	// of v is (0, -2) = -b, after reflection we want b = (0, 2), so, what 
	// vector do I have to add to -b to obtain +b? -b + (b - (-b)) = -b + 2*b = b. 
	// r = v + 2*b = v -2*(dot(v, n))*n.
	return v - 2 * dot(v, n) * n;
}

// Get refracted ray from incident ray v and normal n. rri is relative refraction 
// index. v has to be normalized.
inline vec3 Refract(const vec3& v, const vec3& n, const double& rri)
{
	// -1.0 < cos(theta) < 1.0, so, if, due to numerical precision, it turns out to 
	// be slightly greater than 1.0, use 1.0.
	double cosTheta = std::fmin(dot(-v, n), 1.0);
	// Get perpendicular projection of v on n (v and n have same direction): dot(v, n) * n 
	// (dot(v, n) tells us how much of v lies in n). dot(v, n) = |v|*|n|*cos(theta) = 
	// cos(theta), so, vParall = -cos(theta) * n (v has to point towards the surface). 
	// So, vPerp = v - vParall = v + cos(theta) * n. Refraction changes the angle depending 
	// on the refractive indices: rPerp = rri * (r + cos(theta) * n)
	vec3 vPerp = rri * (v + cosTheta * n);
	// Get parallel projection of v on n: v = vPerp + vParall; Apply Pitagoras theorem: 
	// |v|^2 = |vPerp|^2 + |vParall|^2 (v is the hypotenuse), |v| = 1, so 
	// vParall = sqrt(1 - |vPerp|^2) (this is the vParall's length, but we need to convert 
	// that number into a vector pointing in the opposite direction of the incident normal).
	// vParall = sqrt(1 - |vPerp|^2) * (-n).
	vec3 vParall = -std::sqrt(std::fabs(1.0 - vPerp.LengthSquared())) * n;

	// refracted vector = vPerp + vParall
	return vPerp + vParall;
}

// Generate random unit vector ([-1.0, 1.0]) in unit sphere
inline vec3 RandomUnitVector()
{
	// As long as the vector is not inside the unit sphere, repeat it
	while(true)
	{
		vec3 p = vec3::Random(-1.0, 1.0);
		// p's length = sqrt(px^2 + py^2 + pz^2) <= 1 to get inside sphere, but 
		// px^2 + py^2 + pz^2) <= 1^2, so sqrt is not necessary to check it  
		double lengthSquared = p.LengthSquared();
		// To ensure to avoid underflow to zero when squared (if p components are a 
		// value near to min double value, square operation in p.Length() can underflow 
		// to zero, demnominator = 0), check lengthSquared > 1e-160, so, there is a 
		// "black hole" at center of sphere with radius = 1e-80.
		if(lengthSquared > 1e-160 && lengthSquared <= 1)
		{
			// Normalize random vector inside the sphere
			return p / p.Length();
		}
	}
}

// Generate a random unit vector on hemisphere normal direction
inline vec3 RandomOnHemisphere(const vec3& normal)
{
	// Random unit vector on sphere
	vec3 p = RandomUnitVector();

	// If it is located in the opposite hemisphere, turn it over
	if(dot(p, normal) > 0)
	{
		// It is in the same hemisphere as the normal
		return p;
	}
	else
	{
		return -p;
	}
}

#endif