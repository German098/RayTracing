#include <iostream>

#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
 
int main(int argc, char const* argv[])
{
	// World
	// Materials
	Material* materialGround = new Lambertian(color(0.8, 0.8, 0.0));
	Material* materialCenter = new Lambertian(color(0.1, 0.2, 0.5));
	Material* materialLeft = new Dielectric(1.5 /* ~= glass */);
	// Air sphere inside glass sphere. refractionIndex parameter to the dielectric material 
	// can be interpreted as the ratio of the refractive index of the object divided by the 
	// refractive index of the enclosing medium.
	Material* materialBubble = new Dielectric(1.0 / 1.5);
	Material* materialRight = new Metal(color(0.8, 0.6, 0.2), 1.0);

	// Objects
	HitTableList objectsList;
	objectsList.Add(new Sphere(point3d(0.0, 0.0, -1.2), 0.5, materialCenter));
	objectsList.Add(new Sphere(point3d(0.0, -100.5, -1.0), 100.0, materialGround));
	objectsList.Add(new Sphere(point3d(-1.0, 0.0, -1.0), 0.5, materialLeft));
	objectsList.Add(new Sphere(point3d(-1.0, 0.0, -1.0), 0.4, materialBubble));
	objectsList.Add(new Sphere(point3d(1.0, 0.0, -1.0), 0.5, materialRight));

	// Camera
	Camera camera;
	// Set aspect ratio for image
	camera.AspectRatio(16.0 / 9.0);
	camera.ImageWidth(400);
	camera.SamplesPerPixel(10);
	camera.SamplesMaxDepth(10);
	camera.vFOV = 20;
	camera.lookFrom = vec3(-2, 2, 1);
	camera.lookAt = vec3(0, 0, -1);
	camera.vUp = vec3(0, 1, 0);

	// Render image
	camera.Render(objectsList);

	// Free memory
	delete materialGround;
	delete materialCenter;
	delete materialLeft;
	delete materialRight;
	delete materialBubble;

	return 0;
}