#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <fstream>
#include <ostream>

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
	double Length() const
	{
		return std::sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
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

#endif