#ifndef INTERVAL_H
#define INTERVAL_H

#include "rtweekend.h"

class Interval
{
	private:
		double min;
		double max;

	public:
		// Constructors
		Interval() : min(-INF), max(INF) { }
		Interval(const double &min, const double& max) : min(min), max(max) { }

		// Getters
		const double Max() const { return max; }
		const double Min() const { return min; }

		bool Contains(const double& value) const
		{
			return min <= value && value <= max;
		}

		bool Surrounds(const double& value) const
		{
			return min < value && value < max;
		}
};

#endif
