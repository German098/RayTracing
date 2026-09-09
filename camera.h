#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <ostream>

#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "hittableList.h"

// Construct and dispatch rays into the world and use the results of these rays to 
// construct the rendered image.
class Camera
{
	private:
		// Height in pixels
		int imageHeight;
		int viewportHeight;
		// Camera center (all scene rays will originate from here)
		point3d center;
		// Location on pixel (0, 0)
		point3d pixel00Local;
		// Delta vectors for offset form pixel to pixel
		vec3 pixelDeltaU;
		vec3 pixelDeltaV;
		// Distance between viewport and camera center (orthogonal distnace)
		double focalLength;
		// Color scale factor for a sum of pixel samples (to maintain the 
		// range [0.0, 1.0] in pixel color to write on image)
		double pixelSampleScale;


	public:
		// Aspect ratio image (width over height)
		double aspectRatio = 1.0;
		// Width in pixels
		int imageWidth = 100;
		// Num of random samples per pixel
		unsigned int samplesPerPixel = 10;
		// Max number of ray bounces into scene
		unsigned int samplesMaxDepth = 10;

		void AspectRatio(const double& value) { aspectRatio = value; }
		void ImageWidth(const int& value) { imageWidth = value; }
		void SamplesPerPixel(const double& value) { samplesPerPixel = value; }
		void SamplesMaxDepth(const double& value) { samplesMaxDepth = value; }

	private:
		// Initialize camera's data
		void Initialize()
		{
			// Calculate image height, and ensure that it's at least 1
			imageHeight = int(imageWidth / aspectRatio);
			imageHeight = (imageHeight < 1.0) ? 1.0 : imageHeight;

			// Scale sample depends on num of samples per pixel
			pixelSampleScale = 1.0 / samplesPerPixel;

			// Camera center
			center = point3d(0.0, 0.0, 0.0);

			// Distance between viewport and camera center (orthogonal distnace)
			focalLength = 1.0;

			// Viewport width and height. Note that aspect_ratio is an ideal ratio, which we 
			// approximate as best as possible with the integer-based ratio of image width over 
			// image height. In order for our viewport proportions to exactly match our image 
			// proportions, we use image_width and image_height (real image dimensions) to 
			// determine our final viewport width.
			viewportHeight = 2.0;
			double viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);

			// Vectors across the horizontal and down the vertical viewport edges (right-handed 
			// system: Y up, X right, Z in the direction opposite to the viewport)
			vec3 viewportU = vec3(viewportWidth, 0, 0);
			vec3 viewportV = vec3(0, -viewportHeight, 0);

			// Delta vectors for distance form pixel to pixel
			pixelDeltaU = viewportU / imageWidth;
			pixelDeltaV = viewportV / imageHeight;

			// Location of upper left pixel Pixel(0, 0)
			point3d viewportUpperLeft = center - vec3(0.0, 0.0, focalLength) - viewportU / 2.0 - viewportV / 2.0;
			pixel00Local = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
		}

		// Get ray for pixel(i, j) + random offset [-0.5, 0.5]
		Ray GetRay(const unsigned int& i, const unsigned& j) const
		{
			// Create ray (sample) for pixel(i, j)
			vec3 offset = sampleSquare();
			vec3 pixelSample = pixel00Local + ((offset.X() + i) * pixelDeltaU) + ((offset.Y() + j) * pixelDeltaV);

			// rayDirection has to be a unit vector
			vec3 rayDirection = pixelSample - center;
			vec3 unitRayDirection = unit_vector(rayDirection);

			// origin rays = camera center
			return Ray(center, unitRayDirection);
		}

		// Lineary blend white and bluish (for not hitted object) depending on the height of 
		// the y coordinate of ray direction unit vector and process hitted rays
		color RayColor(const Ray& ray, unsigned int currentDepth, const HitTableList& objectsList)
		{
			HitRecord rec;

			// If we get the ray bounce limit, no more light is gathered
			if(currentDepth == 0)
				return color(0.0, 0.0, 0.0); 
		
			// If true: hint with sphere. infinity() return infinity number (special number 64bits = 0x7FF0000000000000 = +inf).
			// A ray will attempt to accurately calculate the intersection point when it intersects with a surface (rec.pt), 
			// this calculation is susceptible to floating point rounding errors which can cause the intersection point to 
			// be ever so slightly off, so, the origin of the next ray, the ray that is randomly scattered off of the surface, 
			// is unlikely to be perfectly flush with the surface (it might be just above/below, if it's below, then it could 
			// intersect with that surface again. To solve this, we ignore hits that are very close (0.001) to calculated rec.pt.
			if (objectsList.Hit(ray, Interval(0.001, std::numeric_limits<double>::infinity()), rec))
			{	
				// Normal colors
				// normal unit vector from range [-1.0, 1.0] to range [0.0, 1.0]
				//vec3 normal = rec.normal;
				//return 0.5 * (color(normal.X(), normal.Y(), normal.Z()) + 1.0);

				// Simple diffuse
				// normal vector is normalized
				//vec3 direction = RandomOnHemisphere(rec.normal);
				//return 0.5 * RayColor(Ray(rec.pt,  direction), currentDepth - 1, objectsList);

				// Non-uniform Lambertian distribution. In this method a reflected ray is most likely to scatter in a 
				// direction near the surface normal, and less likely to scatter in directions away from the normal. We 
				// create this distribution by adding a random unit vector to the normal vector (and normalize it for next 
				// operations).
				vec3 direction = rec.normal + RandomUnitVector();
				direction = unit_vector(direction);
				return 0.5 * RayColor(Ray(rec.pt,  direction), currentDepth - 1, objectsList);
			}
		
			// Scale direction vector from range [-1.0, 1.0] to range [0.0, 1.0]
			double a = 0.5 * (ray.Direction().Y() + 1.0);
			// If a == 1: ray color = bluish or if a == 0: ray color = white. Linear interpolation:
			return (1.0 - a) * vec3(1.0, 1.0, 1.0) + a * vec3(0.5, 0.7, 1.0);
		}

	public:
		void Render(const HitTableList& objectsList)
		{
			Initialize();

			std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255" << std::endl;

			for (unsigned y = 0; y < imageHeight; y++)
			{
				// Log massage and empty buffer now to show message. (\r set cursor at the begining of the line
				// and erase characters after it with \033[0K) 
				std::clog << "\r\033[0K" << "Scanlines remaining: " << imageHeight - y << "" << std::flush;
				for (unsigned x = 0; x < imageWidth; x++)
				{
					// Create ray for pixel(x, y)
					//point3d currentPixelCenter = pixel00Local + x * pixelDeltaU + y * pixelDeltaV;
					// rayDirection has to be a unit vector
					//vec3 rayDirection = currentPixelCenter - center;
					//vec3 unitRayDirection = unit_vector(rayDirection);
					// origin rays = camera center
					//Ray ray(center, unitRayDirection);
		
					// Return color for a given scene ray (ray per pixel)
					//color pixelColor = RayColor(ray, objectsList);
					color pixelColor(0.0, 0.0, 0.0);
					// Generate samplesPerPixel samples
					//std::cout<<"\n"<<x<<" - "<<y<<std::endl;
					for(unsigned int i = 0; i < samplesPerPixel; i++)
					{
						// Sample
						Ray ray = GetRay(x, y);
						// Return color for a given scene ray (ray per pixel)
						pixelColor += RayColor(ray, samplesMaxDepth, objectsList);
					}
					//std::cout<<"End"<<std::endl;
		
					
					WriteColor(std::cout, pixelSampleScale * pixelColor);
				}
			}
		
			std::clog << "\n\rDone\n" << std::flush;
		}

		// Generates a random sample point within the unit square centered at the origin.
		vec3 sampleSquare() const 
		{
			// Returns vec3 to random point in range [-0.5, 0.5]
			return vec3(RandomDouble() - 0.5, RandomDouble() - 0.5, 0.0);
		}
};

#endif