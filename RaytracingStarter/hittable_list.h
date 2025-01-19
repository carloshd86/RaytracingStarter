#pragma once

#include <memory>
#include <vector>

#include "hittable.h"

using std::make_shared;
typedef std::shared_ptr<hittable> hittable_ptr;

class hittable_list : public hittable
{
public:
	std::vector<hittable_ptr> objects;

	hittable_list() {}
	hittable_list(hittable_ptr object) { add(object); }

	void clear() { objects.clear(); }
	void add(hittable_ptr object)
	{
		objects.push_back(object);
	}

	bool hit(const ray& r, interval ray_t, hit_record& out_rec) const override
	{
		hit_record temp_rec;
		bool hit_anything = false;
		double closest_so_far = ray_t.max;

		for (const auto& object : objects)
		{
			if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec))
			{
				hit_anything = true;
				closest_so_far = temp_rec.t;
				out_rec = temp_rec;
			}
		}

		return hit_anything;
	}
};

