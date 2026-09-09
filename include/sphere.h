#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "ray.h"
#include "hittable.h"

class Sphere : public HitTable 
{
	private:
		point3d center;
		double radius;

	public:
		Sphere(const point3d& center, const double& radius) : center(center), radius(std::fmax(0.0, radius)) { }

		bool Hit(const Ray& ray, const Interval& rayT, HitRecord& rec) const override
		{
			// Sphere formule in terms of vectors: (C - P(t)) * (C - P(t)) = r^2, 
			// C = ceneter; P(t) = vector from point P; r = radius
			// With P(t) = A + t * b => t^2 * b * b - 2 * t * d * (C - A) + (C - A) * (C - A) - r^2 = 0,
			// we have to solve t value (C, A b and r are constants)
			// Solutions: 0 solutions = ray doesn't hit sphere, 2 solutions = hit twice and 1 solution = hit once
			// NOTE: If b = -2 * h, we can siplify: (-b +- std::sqrt(b * b - 4 * a * c)) / (2 * a) => (h +- std::sqrt(h^2 - a * c)) / a
			// and if |ray.Direction()| = 1, so dot(ray.Direction() * ray.Direction()) = |ray.Direction()|^2 = 1, so a = 1
			vec3 ca = center - ray.Origin();
			double a = 1; //dot(ray.Direction(), ray.Direction());
			//double b = -2.0 * dot(ray.Direction(), ca);
			double h = dot(ray.Direction(), ca);
			double c = dot(ca, ca) - radius * radius;
			double discriminant = h * h - a * c;

			// If discriminant of quadratic equation < 0, no hit, else: there is at least one solution, so ray hit 
			// sphere. Find the nearest root that lies in range (rayTMin, rayTMax).
			if (discriminant < 0)
				return false;

			// Resolve equation to get t (value to move the origin A along the ray/line) value
			// NOTE: Denominator >= 0.0, so: -b - std::sqrt(....) < -b + std::sqrt(....) => t1 < t2
			// (t1 closer to the ray origin, so use this one first)
			double t = (h - std::sqrt(discriminant)) / a;
			if(!rayT.Surrounds(t))
			{
				// First solution out of range, so try second solution
				t = (h + std::sqrt(discriminant)) / a;
				if (!rayT.Surrounds(t))
					return false;
			}

			// Update hit data
			rec.t = t;
			// Collision point on sphere
			rec.pt = ray.at(t);
			// Normal at rec.pt (in a sphere vector from sphere's center to rec.pt, CP = P - C, is perpendicular 
			// to the surface and |CP| = radius, so, rec.normal = (P - C) / |CP| = (P - C) / radius)
			vec3 outwardNormal = (rec.pt - center) / radius;
			rec.SetFaceNormal(ray, outwardNormal);

			return true;
		}
};

#endif // !SPHERE_H
