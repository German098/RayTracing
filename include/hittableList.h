#pragma once
#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>
#include "hittable.h"

class HitTableList : public HitTable
{
	private:
		std::vector<HitTable*> objects;

	public:
		// Constructors
		HitTableList() { }
		HitTableList(HitTable* obj) { Add(obj); }

		void Add(HitTable* obj) { objects.push_back(obj); }
		void Clear() { objects.clear(); }

		// Check if ray hit something in range (rayTMin, rayTMax)
		bool Hit(const Ray& ray, const double& rayTMin, const double& rayTMax, HitRecord& rec)
		{
			HitRecord auxRec;
			bool hitAnything = false;
			double tClosestSoFar = rayTMax;

			for (const HitTable* obj : objects)
			{
				// Check for collision in range (rayTMin, tClosestSoFar) for objects array
				if (obj->Hit(ray, rayTMin, tClosestSoFar, auxRec))
				{
					// Hit something
					hitAnything = true;
					// Update the distance at which the hit ocurred
					tClosestSoFar = auxRec.t;
					rec = auxRec;
				}
			}

			return hitAnything;
		}
};

#endif // !define HITTABLE_LIST_H
