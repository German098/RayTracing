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
			// the surrounding material (where the ray is refracted). For example, if you want to render a glass 
			// ball under water, then the glass ball would have an effective refractive index of 1.125. This is 
			// given by the refractive index of glass (1.5) divided by the refractive index of water (1.333). 
			double rri = rec.frontFace ? (1.0 / refractionIndex) : refractionIndex;

			// There are ray angles for which no solution is possible using Snell's law. When a ray enters a medium of lower index of 
			// refraction at a sufficiently glancing angle (far to normal point surface). If a ray pass from glass (n1 ~= 1.5) to vaccum 
			// (n2 ~= 1.0), 1.5*sin(theta1) = 1.0*sin(theta2); If sin(theta1) = A: sin(theta2) = 1.5*A; theta2 = arcsin(1.5*A); 
			// If 1.5*A > 1.0: doesn't exist an angle it's sin(angle) > 1.0. In this cases, we reflect the ray and because in practice 
			// that is usually inside solid objects, it is called total internal reflection. The value of A increases the further the direction of 
			// the ray is from normal surface (but if ray enters a medium of higher index of refraction than it's own, n1/n2 < 1.0, so,
			// no problem). To check if it can happen a total internal reflection, the critical angle (refracted angle at which total internal 
			// reflection begins): sin(theta_critical) = n2/n1; theta_critical = arcsin(n2/n1), if theta2 > theta_critical: total internal reflection,
			// else: refraction.
			// Calculate a angle between incident ray and normal(theta1)
			double cosAngle = std::fmin(dot(-ray.Direction(), rec.normal), 1.0);
			// Using trigonometry identities: sin(angle) = sqrt(1 - cos^2(angle))
			double sinAngle = std::sqrt(1.0 - cosAngle * cosAngle);

			vec3 scatteredRayDirection;
			// If rri * sinAngle > 1.0 || proportion light reflected (percentage) > [0.0, 1.0): reflect ray, else: refract it. Real 
			// glass has reflectivity that varies with angle. When a light wave reaches the interface between two dielectrics (e.g., air → glass), 
			// the energy is generally distributed between: incidnte light = reflected light + transmitted light (refracted light), and the ratio between 
			// the two depends on the angle of incidence and the refractive indices. So, when current ray reffracts, calculate how much of light is reflected, 
			// and if it is higher than random value ([0.0, 1.0)), reflect current ray.
			if (rri * sinAngle > 1.0 || Reflectance(cosAngle, rri) > RandomDouble()) {
				//while(true);
				scatteredRayDirection = Reflect(ray.Direction(), rec.normal);
			}
			else {
				// Get refracted vector
				scatteredRayDirection = Refract(ray.Direction(), rec.normal, rri);
			}
			scatteredRayDirection = unit_vector(scatteredRayDirection);

			// Refracted scattered ray (origin at the point of the incident ray)
			scatteredRay = Ray(rec.pt, scatteredRayDirection);

			return true;
		}

	private:
		// Material's refractive index to calculate the amount that a refracted ray bends
		double refractionIndex;

		// Schlick Approximation allows for the calculation of the proportion of light reflected and refracted at a surface without having to calculate the 
		// exact Fresnel equations. The Fresnel effect causes a surface to reflect more light when viewed from a very shallow angle (far from normla vector).
		static double Reflectance(const double& cosAngle, double ri) {
			// R(theta) = R0 + (1 - R0) * (1 - cos(theta))^5 . Theta = angle between light ray and normal, R0 = reflectance percentage when light hit perpendicular 
			// to surface (theta = 0º) = ((n1 - n2) / (n1 + n2))^2, rest of the light is transmitted (refracted). n1 = vaccum refractive index.
			double r0 = (1.0 - ri) / (1.0 + ri);
			r0 = r0 * r0;
			return r0 + (1 - r0) * std::pow((1 - cosAngle), 5);
		}
};

#endif