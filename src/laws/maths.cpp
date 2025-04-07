// maths.cpp
#include "maths.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float random(int precision)
{
    float x = 0.0f;
    for (int i = 1; i <= precision; i++)
    {
        int bit = std::rand() % 2;
        x += static_cast<float>(bit) / std::pow(2, i);
    }
    return x;
}

float generateGaussian()
{
    float u1 = static_cast<float>(rand()) / RAND_MAX;
    float u2 = static_cast<float>(rand()) / RAND_MAX;

    float z0 = std::sqrt(-2.0f * std::log(u1)) * std::cos(2 * M_PI * u2);

    float mean   = 0.1f;
    float stddev = 0.03f;

    float gray = std::clamp(mean + stddev * z0, 0.0f, 0.2f);
    return gray;
}

float gaussian_approx(int n)
{
    float sum = 0.0f;
    for (int i = 0; i < n; ++i)
    {
        sum += random(20);
    }
    float mean   = n / 2.0f;
    float stddev = std::sqrt(n / 12.0f);
    float z      = (sum - mean) / stddev;

    float result = 0.5f + z * 0.15f;
    return std::clamp(result, 0.0f, 1.0f);
}

ImVec4 generateFancyDarkColor()
{
    float base = 0.1f;
    float r    = std::clamp(base + gaussian_approx() * 0.2f, 0.0f, 1.0f);
    float g    = std::clamp(base + gaussian_approx() * 0.2f, 0.0f, 1.0f);
    float b    = std::clamp(base + gaussian_approx() * 0.2f, 0.0f, 1.0f);

    return ImVec4(r, g, b, 1.0f);
}

int tirageExponentialIndex(float lambda)
{
    float u = random(20); // Ton générateur binaire
    float x = -std::log(u) / lambda;
    return std::min(static_cast<int>(x), 3);
}

bool bernoulli(float p, int precision)
{
    float r = random(precision);
    return r < p;
}