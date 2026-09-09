#ifndef VEC3_H
#define VEC3_H

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