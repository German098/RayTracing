#ifndef COLOR_H
#define COLOR_H

#include <ostream>
#include "vec3.h"

#include <iostream>
#include "interval.h"

void writeColor(std::ostream& out, const color& pixel_color)
{
	Interval interval(0.0, 1.0);
	if(!interval.Contains(pixel_color.X()) || !interval.Contains(pixel_color.Y()) || !interval.Contains(pixel_color.Z()))
	{	
		// If this happens, why? and do clamp function
		std::cout<<"while: "<<pixel_color.X()<<" "<<pixel_color.Y()<<" "<<pixel_color.Z()<<std::endl;
		while(true);
	}

	// Translate components in range [0.0, 1.0] to byte range [0, 255]. Scale 
	// to integer values between 0 and 255 (guarantees that any value strictly less 
	// than 1.0 and close to it can result in 255)
	double rbyte = int(255.999 * pixel_color.X());
	double gbyte = int(255.999 * pixel_color.Y());
	double bbyte = int(255.999 * pixel_color.Z());

	// Write out the pixel color components
	out << rbyte << " " << gbyte << " " << bbyte << "\n";
}

#endif