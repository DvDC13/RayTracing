#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385f;

// Returns a random real in [0,1).
inline double RandomDouble()
{
    static std::uniform_real_distribution<double> distribution(0.0f, 1.0f);
    static std::mt19937 generator;
    return distribution(generator);
}

// Returns a random real in [min,max).
inline double RandomDouble(double min, double max) { return min + (max - min) * RandomDouble(); }

// Clamp the value x to be in the range [min,max].
inline double Clamp(double x, double min, double max)
{
    if (x < min) return min;
    if (x > max) return max;
    return x;
}