#pragma once

#include "hittable.h"
#include "color.h"
#include "material.h"

class camera {
public:
	camera(double aspect_ratio, int image_width, int samples_per_pixel, int max_depth, double vfov, point3 look_from, point3 look_at, vec3 up_vector, double defocus_angle, double focus_dist)
		: aspect_ratio(aspect_ratio)
		, image_width(image_width)
		, samples_per_pixel(samples_per_pixel)
		, max_depth(max_depth)
		, vfov(vfov)
		, look_from(look_from)
		, look_at(look_at)
		, up_vector(up_vector)
		, defocus_angle(defocus_angle)
		, focus_dist(focus_dist)
	{
	}

	camera() : camera(1.0, 100, 10, 10, 90.0, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, -1.0 }, { 0.0, 1.0, 0.0 }, 0.0, 10.0) {}

	void render(const hittable& world)
	{
		std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

		for (int j = 0; j < image_height; ++j)
		{
			std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
			for (int i = 0; i < image_width; ++i)
			{
				color pixel_color = COLOR_BLACK;
				for (int sample = 0; sample < samples_per_pixel; ++sample)
				{
					ray r = get_ray(i, j);
					pixel_color += ray_color(r, max_depth, world);
				}
				write_color(std::cout, pixel_samples_scale * pixel_color);


				// pre antialiasing

				//point3 pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
				//vec3 ray_direction = (pixel_center - center).get_safe_normal();
				////vec3 ray_direction = pixel_center - center;
				//ray r(center, ray_direction);

				//color pixel_color = ray_color(r, world);
				//write_color(std::cout, pixel_color);
			}
		}

		std::clog << "\rDone.                 \n";
	}

	void initialize()
	{
		image_height = int(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		pixel_samples_scale = 1.0 / samples_per_pixel;

		center = look_from;

		// Viewport dimensions
		double theta = degrees_to_radians(vfov);
		double h = std::tan(theta / 2.0);
		double viewport_height = 2.0 * h * focus_dist;
		double viewport_width = viewport_height * (double(image_width) / double(image_height));

		// Calculate the u,v,w unit basis vectors for the camera coordinate frame.
		w = (look_from - look_at).get_safe_normal();
		u = cross(up_vector, w).get_safe_normal();
		v = cross(w, u);

		// Calculate the vectors across the horizontal and down the vertical viewport edges.
		vec3 viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
		vec3 viewport_v = viewport_height * -v;  // Vector down viewport vertical edge

		// Calculate the horizontal and vertical delta vectors from pixel to pixel.
		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		// Calculate the location of the upper left pixel.
		vec3 viewport_upper_left = center - (focus_dist * w) - (viewport_u * 0.5) - (viewport_v * 0.5);
		pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

		// Calculate the camera defocus disk basis vectors.
		double defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle * 0.5));
		defocus_disk_u = u * defocus_radius;
		defocus_disk_v = v * defocus_radius;
	}

private:
	double aspect_ratio;
	int image_width;                          // Rendered image width in pixel count
	int samples_per_pixel;                    // Count of random samples for each pixel
	int max_depth = 10;                       // Maximum number of ray bounces into scene
	double vfov = 90.0;                       // Vertical view angle (field of view)
	point3 look_from = point3(0.0, 0.0, 0.0); // Point camera is looking from
	point3 look_at = point3(0.0, 0.0, -1.0);  // Point camera is looking at
	vec3   up_vector = vec3(0.0, 1.0, 0.0);   // Camera-relative "up" direction
	double defocus_angle = 0.0;               // Variation angle of rays through each pixel
	double focus_dist = 10.0;                 // Distance from camera look_from point to plane of perfect focus

	int    image_height = 0;          // Rendered image height
	double pixel_samples_scale = 0.0; // Color scale factor for a sum of pixel samples
	point3 center;                    // Camera center
	point3 pixel00_loc;               // Location of pixel 0, 0
	vec3   pixel_delta_u;             // Offset to pixel to the right
	vec3   pixel_delta_v;             // Offset to pixel below
	vec3   u, v, w;                   // Camera frame basis vectors
	vec3   defocus_disk_u;            // Defocus disk horizontal radius
	vec3   defocus_disk_v;            // Defocus disk vertical radius

	color ray_color(const ray& r, const int depth, const hittable& world) const
	{
		// If we've exceeded the ray bounce limit, no more light is gathered.
		if (depth <= 0)
		{
			return COLOR_BLACK;
		}

		hit_record rec;
		// Using a t slightly above the surface to avoid floating point error of being able to be just below the surface if using 0.0
		if (world.hit(r, interval(0.001, rt_infinity), rec))
		{
			ray scattered;
			color attenuation;
			if (rec.hit_material->scatter(r, rec, attenuation, scattered))
			{
				return attenuation * ray_color(scattered, depth - 1, world);
			}
			else
			{
				return COLOR_BLACK;
			}
		}

		double a = 0.5 * (r.direction().y() + 1.0);
		//double a = 0.5 * (r.direction().get_safe_normal().y() + 1.0);
		return (1.0 - a) * COLOR_WHITE + a * color(0.5, 0.7, 1.0);
	}

	ray get_ray(int i, int j) const
	{
		// Construct a camera ray originating from the defocus disk and directed at a randomly
		// sampled point around the pixel location i, j.

		vec3 offset = sample_square();
		vec3 pixel_sample = pixel00_loc
			+ ((i + offset.x()) * pixel_delta_u)
			+ ((j + offset.y()) * pixel_delta_v);

		point3 ray_origin = (defocus_angle <= 0.0) ? center : defocus_disk_sample();
		vec3 ray_direction = (pixel_sample - ray_origin).get_safe_normal();

		return ray(ray_origin, ray_direction);
	}

	vec3 sample_square() const
	{
		// Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
		return vec3(random_double() - 0.5, random_double() - 0.5, 0.0);
	}

	point3 defocus_disk_sample() const
	{
		// Returns a random point in the camera defocus disk.
		vec3 p = random_in_unit_disk();
		return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
	}

};