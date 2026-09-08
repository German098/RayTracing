#include <iostream>

#include "rtweekend.h"
#include "hittableList.h"
#include "sphere.h"
#include "camera.h"
 
int main(int argc, char const* argv[])
{
	// World
	HitTableList objectsList;
	objectsList.Add(new Sphere(point3d(0.0, 0.0, -1.0), 0.5));
	objectsList.Add(new Sphere(point3d(0.0, -100.5, -1.0), 100.0));

	// Camera
	Camera camera;
	// Set aspect ratio for image
	camera.AspectRatio(16.0 / 9.0);
	camera.ImageWidth(400);
	camera.SamplesPerPixel(100);

	// Render image
	camera.Render(objectsList);

	return 0;
}