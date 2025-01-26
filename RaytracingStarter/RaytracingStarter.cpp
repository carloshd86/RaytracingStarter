// RaytracingStarter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "camera.h"
#include "color.h"
#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "rtstarterutils.h"
#include "material.h"

// https://raytracing.github.io/books/RayTracingInOneWeekend.html continue in 12.2

typedef std::shared_ptr<material> material_ptr;

int main()
{
	hittable_list world;

	material_ptr material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	material_ptr material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
	// refraction index of glass => 1.50-1.54
	// refraction index of window glass => 1.52
	// refraction index of air => 1.0
	material_ptr material_left = make_shared<dielectric>(1.52);
	material_ptr material_bubble = make_shared<dielectric>(1.00 / 1.52);
	material_ptr material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);
	material_ptr material_blue = make_shared<lambertian>(COLOR_BLUE);
	material_ptr material_red = make_shared<lambertian>(COLOR_RED);

	double sphere_radius = std::cos(pi / 4);
	world.add(make_shared<sphere>(point3(-sphere_radius, 0.0, -1.0), sphere_radius, material_blue));
	world.add(make_shared<sphere>(point3(sphere_radius, 0.0, -1.0), sphere_radius, material_red));

	/*world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
	world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));*/

	camera cam(16.0 / 9.0, 400, 100, 50, 90.0);
	cam.initialize();
	cam.render(world);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
