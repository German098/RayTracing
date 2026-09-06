#ifndef COLOR_H
#define COLOR_H

#include <iostream>

#include "vec3.h"

void write_color(std::ostream& out, const color& pixel_color)
{
	// Translate components in range [0.0, 1.0] to byte range [0, 255]
	double rbyte = int(255.999 * pixel_color.X());
	double gbyte = int(255.999 * pixel_color.Y());
	double bbyte = int(255.999 * pixel_color.Z());

	// Write out the pixel color components
	out << rbyte << " " << gbyte << " " << bbyte <<"\n";
}

#endif