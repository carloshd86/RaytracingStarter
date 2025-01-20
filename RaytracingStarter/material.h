#pragma once

#include "hittable.h"
#include "color.h"

class material
{
public:
	virtual ~material() = default;

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const
	{
		return false;
	}
};

// Lambertian material (rays randomly scattered)
class lambertian : public material
{
public:
	lambertian(const color& albedo) : albedo(albedo) {}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		vec3 scatter_direction = rec.normal + random_unit_vector();
		// Avoid scatter direction being zero as it may lead to errors
		if (scatter_direction.near_zero())
		{
			scatter_direction = rec.normal;
		}
		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}

private:
	color albedo;
};


// Metal material (rays reflected)
class metal : public material {
public:
	metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1.0 ? fuzz : 1.0) {}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		// Not normalizing the reflected ray as it will be normal because direction is already normal
		vec3 reflected = reflect(r_in.direction(), rec.normal) + (fuzz * random_unit_vector());
		scattered = ray(rec.p, reflected);
		attenuation = albedo;

		// Not scattering if the fuzz made the ray go inside the hittable
		return (dot(scattered.direction(), rec.normal) > 0);
	}

private:
	color albedo;
	double fuzz;
};