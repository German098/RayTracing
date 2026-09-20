#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "ray.h"
#include "vec3.h"
#include <iostream>

class Material
{
	public:
		// Virtual destructor and generate an automatic implementation
		virtual ~Material() = default;

		// If scattered, how much the ray should be attenuated?
		virtual bool Scatter(const Ray& ray, const HitRecord& rec, color& attenuation, Ray& scatteredRay) const
		{
			return false;
		}
};

class Lambertian : public Material
{
	public:
		Lambertian(const color& albedo) : albedo(albedo) { }

		// Lambertian reflectance/diffuse can either always scatter and attenuate light according to its 
		// reflectance R, or it can sometimes scatter (with probability 1−R) with no attenuation (where a ray 
		// that isn't scattered is just absorbed into the material) because you have already used probability 
		// to represent the average loss. It could also be a mixture of both those strategies.
		bool Scatter(const Ray& ray, const HitRecord& rec, color& attenuation, Ray& scatteredRay) const override
		{
			// Non-uniform Lambertian distribution. In this method a reflected ray is most likely to scatter in a 
			// direction near the surface normal, and less likely to scatter in directions away from the normal. We 
			// create this distribution by adding a random unit vector to the normal vector (and normalize it for next 
			// operations).
			vec3 scatter_direction = rec.normal + RandomUnitVector();
			// Avoid scatter_direction = vec3(0.0, 0.0, 0.0)
			if(scatter_direction.NearZero())
				scatter_direction = rec.normal;

			scatter_direction = unit_vector(scatter_direction);
			scatteredRay = Ray(rec.pt, scatter_direction);
			// How much you reduce the ray contribution?. We could scatter with some fixed probability p 
			// and have attenuation be albedo/p.
			attenuation = albedo;

			// Ray was scattered
			return true;
		}

	private:
		// What fraction does this material reflect/What fraction of the incident light is reflected?
		color albedo;
};

class Metal : public Material
{
	public:
		Metal(const color& albedo, const double& fuzz) : albedo(albedo), fuzz((fuzz >= 0.0 && fuzz <= 1.0) ? fuzz : 1.0) { }

		bool Scatter(const Ray& ray, const HitRecord& rec, color& attenuation, Ray& scatteredRay) const override
		{
			// Reflected ray normalized
			vec3 reflectDireciton = Reflect(ray.Direction(), rec.normal);
			// Randomize reflected direction ray by using small fuzzy sphere
			reflectDireciton = reflectDireciton + (fuzz * RandomUnitVector());
			scatteredRay = Ray(rec.pt, reflectDireciton);
			attenuation = albedo;

			// If the radius of the fuzzy sphere is large, the reflected ray could point in the opposite 
			// direction; If so, return no scattering (angle between reflect ray and normal is not in 
			// range (-90º, 90º)).
			return dot(reflectDireciton, rec.normal) > 0;
		}

	private:
		// What fraction does this material reflect/What fraction of the incident light is reflected?
		color albedo;
		// Scaling factor (radius of fuzzy sphere) to increase or decrease the fuzz effect
		double fuzz;
};

class Dielectric : public Material 
{
	public:
		Dielectric(const double& refractionIndex) : refractionIndex(refractionIndex) { }

		bool Scatter(const Ray& ray, const HitRecord& rec, color& attenuation, Ray& scatteredRay) const
		{
			// Keep attenuation (ray color/energy) at same value
			attenuation = color(1.0, 1.0, 1.0);
			// Relative refraction index: refractive index of the material of the object from which the 
			// ray originates (assume it originates in the vacuum =~ 1.0), divided by the refractive index of 
			// the surrounding material (where the ray is refracted). 
			double rri = rec.frontFace ? (1.0 / refractionIndex) : refractionIndex;

			// Get refracted vector
			vec3 refracted = Refract(ray.Direction(), rec.normal, rri);
			refracted = unit_vector(refracted);

			// Refracted scattered ray (origin at the point of the incident ray)
			scatteredRay = Ray(rec.pt, refracted);
			std::cout<<"ray: "<<ray.Direction().X()<<" "<<ray.Direction().Y()<<" "<<ray.Direction().Z()<<std::endl;
			std::cout<<"scatteredRay: "<<scatteredRay.Direction().X()<<" "<<scatteredRay.Direction().Y()<<" "<<scatteredRay.Direction().Z()<<std::endl;

			return true;
		}

	private:
		// Material's refractive index to calculate the amount that a refracted ray bends
		double refractionIndex;

};

#endif