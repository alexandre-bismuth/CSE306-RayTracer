#ifndef UTILS_H
#define UTILS_H

#include <random>
#include <omp.h>
#include "vector.h"

#define M_PI 3.14159265358979323846264338327950288
#define MAX_THREADS 10 // Obtained from running omp_get_max_threads()

static double sqr(double x) { return x * x; };
static double clamp(double x, double floor, double ceil) { return std::max(std::min(ceil, x), floor); }

static std::default_random_engine engine[MAX_THREADS];
static std::uniform_real_distribution<double> uniform;

struct IntersectionInfo
{
    Vector inter, interNormal;
    double dist, alpha, beta, gamma;
    int objectId, triangleId;
};

#endif
