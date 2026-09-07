#include <iostream>

#include "sphere.h"

HitRecord dataHit;
Sphere sphere(point3d(0.0, 0.0, -1.0), 0.5);

// Lineary blend white and bluish depending on the height of the y coordinate 
// of ray direction unit vector
color ray_color(const Ray& ray)
{	
	point3d sphere_center = point3d(0.0, 0.0, -1.0);

	// If true: hint with sphere
	if (sphere.Hit(ray, 0.0, 1000000.0, dataHit))
	{
		// Calculate the normal vector at the point on the surface of the sphere
		// and normalize [-1.0, 1.0]
		vec3 aux = ray.at(dataHit.t) - sphere_center;
		vec3 normal = unit_vector(ray.at(dataHit.t) - sphere_center);
		// From range [-1.0, 1.0] to range [0.0, 1.0]
		return 0.5 * (color(normal.X(), normal.Y(), normal.Z()) + 1.0);
	}

	// Scale direction vector from range [-1.0, 1.0] to range [0.0, 1.0]
	double a = 0.5 * (ray.Direction().Y() + 1.0);
	// If a == 1: ray color = bluish or if a == 0: ray color = 
	// white. Linear interpolation:
	return (1.0 - a) * vec3(1.0, 1.0, 1.0) + a * vec3(0.5, 0.7, 1.0);
}


int main(int argc, char const* argv[])
{
	// Image
	// Aspect ratio image
	double aspect_ratio = 16.0 / 9.0;
	int image_width = 400;
	// Calculate image height, and ensure that it's at least 1
	int image_height = int(image_width / aspect_ratio);

	// Camera
	// Viewport width and height. Note that aspect_ratio is an ideal ratio, which we 
	// approximate as best as possible with the integer-based ratio of image width over 
	// image height. In order for our viewport proportions to exactly match our image 
	// proportions, we use image_width and image_height (real image dimensions) to 
	// determine our final viewport width.
	double viewport_height = 2.0;
	double viewport_width = viewport_height * (double(image_width) / image_height);
	//std::cout<<"Wievport: "<<viewport_width<<" "<<viewport_height<<std::endl;
	// Camera center (all scene rays will originate from here)
	point3d camera_center = point3d(0.0, 0.0, 0.0);
	// Distance between viewport and camera center (orthogonal distnace)
	double focal_length = 1.0;
	// Vectors across the horizontal and down the vertical viewport 
	// edges (right-handed system: Y up, X right, Z in the direction opposite to the viewport)
	vec3 viewport_u = vec3(viewport_width, 0, 0);
	vec3 viewport_v = vec3(0, -viewport_height, 0);
	// Delta vectors for distance form pixel to pixel
	vec3 pixel_delta_u = viewport_u / image_width;
	vec3 pixel_delta_v = viewport_v / image_height;
	//std::cout<<"pixel_delta_u: "<<pixel_delta_u.X()<<" "<<pixel_delta_u.Y()<<" "<<pixel_delta_u.Z()<<std::endl;
	//std::cout<<"pixel_delta_v: "<<pixel_delta_v.X()<<" "<<pixel_delta_v.Y()<<" "<<pixel_delta_v.Z()<<std::endl;

	// Location of upper left pixel Pixel(0, 0)
	point3d viewport_upper_left = camera_center - vec3(0.0, 0.0, focal_length) - viewport_u / 2.0 - viewport_v / 2.0;
	point3d pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	// Render
	// File type, size and max component value
	std::cout << "P3\n" << image_width << " " << image_height << "\n255" << std::endl;

	// Pixels
	for (unsigned y = 0; y < image_height; y++)
	{
		// Log massage and empty buffer now to show message
		std::clog << "\rScanlines remaining: " << image_height - y << "" << std::flush;
		for (unsigned x = 0; x < image_width; x++)
		{
			if (y == image_height / 2 && x == image_width / 2)
				int x = 0;

			// Create ray for pixel(x, y)
			point3d current_pixel_center = pixel00_loc + x * pixel_delta_u + y * pixel_delta_v;
			// ray_direction hast to be a unit vector
			vec3 ray_direction = current_pixel_center - camera_center;
			vec3 unit_ray_direction = unit_vector(ray_direction);
			// origin rays = camera center
			Ray ray(camera_center, unit_ray_direction);

			// Return color for a given scene ray (ray per pixel)
			color pixel_color = ray_color(ray);

			// Scale to integer values between 0 and 255 (guarantees that any value strictly less than 1.0 and close to it can result in 255)
			write_color(std::cout, pixel_color);
		}
	}

	std::clog << "\rDone\n" << std::flush;

	return 0;
}