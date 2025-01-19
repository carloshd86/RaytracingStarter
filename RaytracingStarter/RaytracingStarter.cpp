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

// https://raytracing.github.io/books/RayTracingInOneWeekend.html continue in 9.4

int main()
{
	hittable_list world;

	world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5));
	world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0));

	camera cam(16.0 / 9.0, 400, 100, 50);
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
