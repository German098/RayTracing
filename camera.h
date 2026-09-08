#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include "rtweekend.h"
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

		void AspectRatio(const double& value) { aspectRatio = value; }
		void ImageWidth(const int& value) { imageWidth = value; }
		void SamplesPerPixel(const double& value) { samplesPerPixel = value; }

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
		// the y coordinate of ray direction unit vector 
		color RayColor(const Ray& ray, const HitTableList& objectsList)
		{
			HitRecord rec;
		
			// If true: hint with sphere. infinity() return infinity number (special number 64bits = 0x7FF0000000000000 = +inf)
			if (objectsList.Hit(ray, Interval(0.0, std::numeric_limits<double>::infinity()), rec))
			{
				// normal unit vector from range [-1.0, 1.0] to range [0.0, 1.0]
				vec3 normal = rec.normal;
				return 0.5 * (color(normal.X(), normal.Y(), normal.Z()) + 1.0);
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
				// Log massage and empty buffer now to show message
				std::clog << "\rScanlines remaining: " << imageHeight - y << "" << std::flush;
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
						Ray ray = GetRay(x, y);
						// Return color for a given scene ray (ray per pixel)
						pixelColor += RayColor(ray, objectsList);
					}
					//std::cout<<"End"<<std::endl;
		
					
					writeColor(std::cout, pixelSampleScale * pixelColor);
				}
			}
		
			std::clog << "\rDone\n" << std::flush;
		}

		// Generates a random sample point within the unit square centered at the origin.
		vec3 sampleSquare() const 
		{
			// Returns vec3 to random point in range [-0.5, 0.5]
			return vec3(RandomDouble() - 0.5, RandomDouble() - 0.5, 0.0);
		}
};

#endif