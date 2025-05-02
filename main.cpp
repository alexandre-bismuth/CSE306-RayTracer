#define _CRT_SECURE_NO_WARNINGS 1

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "headers/stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"

#define M_PI 3.14159265358979323846264338327950288

#include "headers/scene.h"
#include "headers/utils.h"
#include <iostream>

using namespace std::chrono;

int main(int argc, char **argv)
{
	if (argc != 2)
		return 1;

	const std::string mode = argv[1];
	if (mode != "spheres" && mode != "cat" && mode != "combined")
		return 1;

	// Time initialization
	const steady_clock::time_point start = steady_clock::now();

	// Orthonormal basis for the camera
	const Vector u(1, 0, 0), v(0, 1, 0);

	// Global variable definition
	const int imageWidth = 512, imageHeight = 512;
	const Vector cameraOrigin(0, 0, 55), lightPosition(-10, 20, 40);
	const unsigned long lightIntensity = 1E7;
	const unsigned int raysPerPixel = 128;
	const unsigned char bounceNumber = 5;
	const double fieldOfView = 60. * M_PI / 180.;
	const double aperture = (mode == "combined") ? 0.75 : 0.;
	const double focusDist = 25.;
	const bool useSpheres = (mode == "spheres" || mode == "combined");
	const bool useCat = (mode == "cat" || mode == "combined");

	// Environment Setup
	Scene scene(lightPosition, lightIntensity, useSpheres, useCat);
	scene.initEnvironment();
	std::vector<unsigned char> image(imageWidth * imageHeight * 3, 0);

#pragma omp parallel for schedule(dynamic, 1)
	for (int i = 0; i < imageHeight; i++)
	{
		for (int j = 0; j < imageWidth; j++)
		{
			Vector accumulatedColor;
			for (int s = 0; s < raysPerPixel; s++)
			{
				// Anti-aliasing
				const unsigned char threadId = omp_get_thread_num();
				const double radius = uniform(engine[threadId]);
				const double angle = 2. * M_PI * uniform(engine[threadId]);
				const double jitterX = sqrt(-log(radius)) * cos(angle) * 0.25;
				const double jitterY = sqrt(-log(radius)) * sin(angle) * 0.25;
				const double dist = -imageWidth / (2. * tan(fieldOfView / 2.));
				Vector rayDir(j - imageWidth / 2. + 0.5 + jitterX, imageHeight / 2. - i + 0.5 + jitterY, dist);
				rayDir.normalize();

				// Depth of Field
				const double lensR = aperture * sqrt(radius);
				const Vector focalPt = cameraOrigin + rayDir * focusDist;
				const Vector lensOffset = lensR * cos(angle) * u + lensR * sin(angle) * v;

				const Vector rayOrig = cameraOrigin + lensOffset;
				Ray ray(rayOrig, normalize(focalPt - rayOrig));

				accumulatedColor += scene.getColor(ray, bounceNumber);
			}

			const Vector finalColor = accumulatedColor / double(raysPerPixel);

			image[(i * imageWidth + j) * 3 + 0] = std::min(255., 255. * std::pow(finalColor[0] / 255., 1 / 2.2));
			image[(i * imageWidth + j) * 3 + 1] = std::min(255., 255. * std::pow(finalColor[1] / 255., 1 / 2.2));
			image[(i * imageWidth + j) * 3 + 2] = std::min(255., 255. * std::pow(finalColor[2] / 255., 1 / 2.2));
		}
	}
	std::string file = "output/" + mode + ".png";
	stbi_write_png(file.c_str(), imageWidth, imageHeight, 3, &image[0], 0);

	// Timing final computations
	const unsigned long elapsed = duration_cast<milliseconds>(steady_clock::now() - start).count();
	std::cout << elapsed / 1000.0 << " seconds" << std::endl;
	return 0;
}