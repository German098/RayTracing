#ifndef COLOR_H
#define COLOR_H

#include <cmath>
#include <ostream>
#include "vec3.h"

#include <iostream>
#include "interval.h"

// Transfrom component value to gamma space
inline double LinearToGamma(const double& value)
{
	return /*std::sqrt(value);*/ std::pow(value, 1 / 2.2);
}

void WriteColor(std::ostream& out, const color& pixelColor)
{
	Interval interval(0.0, 1.0);
	if(!interval.Contains(pixelColor.X()) || !interval.Contains(pixelColor.Y()) || !interval.Contains(pixelColor.Z()))
	{	
		// If this happens, why? and do clamp function
		std::cout<<"while: "<<pixelColor.X()<<" "<<pixelColor.Y()<<" "<<pixelColor.Z()<<std::endl;
		while(true);
	}

	// Apply from linear to gamma correction 
	double r = LinearToGamma(pixelColor.X());
	double g = LinearToGamma(pixelColor.Y());
	double b = LinearToGamma(pixelColor.Z());

	// Translate components in range [0.0, 1.0] to byte range [0, 255]. Scale 
	// to integer values between 0 and 255 (guarantees that any value strictly less 
	// than 1.0 and close to it can result in 255)
	double rbyte = int(255.999 * r);
	double gbyte = int(255.999 * g);
	double bbyte = int(255.999 * b);

	// Write out the pixel color components
	out << rbyte << " " << gbyte << " " << bbyte << "\n";
}

#endif