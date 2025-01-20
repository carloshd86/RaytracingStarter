#pragma once

#include "hittable.h"

class material;

class sphere : public hittable
{
public:
	sphere(const point3& center, double radius, std::shared_ptr<material> sphere_material)
		: center(center)
		, radius(std::fmax(0.0, radius))
		, sphere_material(sphere_material)
	{
	}

	bool hit(const ray& r, interval ray_t, hit_record& out_rec) const override
	{
		vec3 oc = center - r.origin();
		double a = r.direction().length_squared();
		double h = dot(r.direction(), oc);
		double c = oc.length_squared() - radius * radius;

		double discriminant = h * h - a * c;
		if (discriminant < 0.0)
		{
			return false;
		}

		double sqrtd = std::sqrt(discriminant);

		// Find the nearest root that lies in the acceptable range.
		double root = (h - sqrtd) / a;
		if (!ray_t.surrounds(root))
		{
			root = (h + sqrtd) / a;
			if (!ray_t.surrounds(root))
			{
				return false;
			}
		}

		out_rec.t = root;
		out_rec.p = r.at(out_rec.t);
		vec3 outward_normal = (out_rec.p - center) / radius;
		out_rec.set_face_normal(r, outward_normal);
		out_rec.hit_material = sphere_material;

		return true;
	}

private:
	point3 center;
	double radius;
	std::shared_ptr<material> sphere_material;
};

