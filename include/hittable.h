#ifndef HITTABLE_H
#define HITTABLE_H

#include "vec3.h"
#include "ray.h"
#include "interval.h"

// Record hit data
class HitRecord
{
	public:
		// Hit point on sphere
		point3d pt;
		// Normal in pt (always point against the incident ray)
		vec3 normal;
		// Units moved to get P from ray's origin (camera center)
		double t;
		// If true: incident ray is outside the sphere, else: inside it
		bool frontFace;

		void SetFaceNormal(const Ray& ray, const vec3& outwardNormal)
		{
			// if dot(ray dir, normal) < 0: point in opposite directions, else: 
			// invert normal 
			frontFace = dot(ray.Direction(), outwardNormal);
			normal = frontFace ? outwardNormal : -outwardNormal;
		}
};

// If another class inherits HitTable, it can be hit, so has to implement Hit function.
class HitTable
{
	public:
		virtual ~HitTable() = default;

		// Have to be implemented per class
		virtual bool Hit(const Ray& ray, const Interval& rayT, HitRecord& rec) const = 0;
};

#endif // !HITTABLE_H
