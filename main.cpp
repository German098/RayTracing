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
	Material* material_ground = new Lambertian(color(0.8, 0.8, 0.0));
	Material* material_center = new Lambertian(color(0.1, 0.2, 0.5));
	Material* material_left = new Dielectric(1.5 /* ~= glass */);
	Material* material_right = new Metal(color(0.8, 0.6, 0.2), 1.0);

	// Objects
	HitTableList objectsList;
	//objectsList.Add(new Sphere(point3d(0.0, 0.0, -1.0), 0.5, material_center));
	objectsList.Add(new Sphere(point3d(0.0, -100.5, -1.0), 100.0, material_ground));
	objectsList.Add(new Sphere(point3d(-1.0, 0.0, -1.0), 0.5, material_left));
	//objectsList.Add(new Sphere(point3d(1.0, 0.0, -1.0), 0.5, material_right));

	// Camera
	Camera camera;
	// Set aspect ratio for image
	camera.AspectRatio(16.0 / 9.0);
	camera.ImageWidth(400);
	camera.SamplesPerPixel(100);
	camera.SamplesMaxDepth(50);

	// Render image
	camera.Render(objectsList);

	// Free memory
	delete material_ground;
	delete material_center;
	delete material_left;
	delete material_right;

	return 0;
}