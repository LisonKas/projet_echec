// maths.cpp
#include "maths.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime> // pour std::time
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Nombre aléatoire à virgule flottante (bit par bit) sur [0, 1]

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

// Loi normale pour la couleur des cases foncées
float gaussian(int n)
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

// Loi exponentielle pour la promotion aléatoire
float exponential(float lambda)
{
    float u = random(20); // Génère un float entre 0 et 1
    return -std::log(u) / lambda;
}

bool bernoulli(float p, int precision)
{
    float r = random(precision);
    return r < p;
}

float random_uniform(int precision)
{
    std::srand(static_cast<unsigned int>(std::time(0))); // Réinitialisation à chaque appel
    float x = 0.0f;
    for (int i = 1; i <= precision; ++i)
    {
        int bit = std::rand() % 2;
        x += static_cast<float>(bit) / std::pow(2, i);
    }
    return x;
}

int select_index_uniform(int nb_choices)
{
    float r = random_uniform(); // ∈ [0, 1)
    for (int i = 0; i < nb_choices; ++i)
    {
        if (r < static_cast<float>(i + 1) / nb_choices)
        {
            return i;
        }
    }
    return nb_choices - 1; // Par sécurité
}

float logNormalMapped(float mu, float sigma, float minSpeed, float maxSpeed, int precision)
{
    std::srand(static_cast<unsigned int>(std::time(0))); // Réinitialisation à chaque appel

    float u1 = random(precision);
    float u2 = random(precision);

    // Loi normale centrée réduite via Box-Muller
    float z = std::sqrt(-2.0f * std::log(u1)) * std::cos(2 * M_PI * u2);

    // Loi log-normale
    float raw = std::exp(mu + sigma * z);

    // Clamper pour éviter les valeurs trop extrêmes
    float clamped = std::clamp(raw, 0.1f, 3.0f); // bornes raisonnables pour la vitesse brute

    // Mapping linéaire dans [minSpeed, maxSpeed]
    float normalized  = (clamped - 0.1f) / (3.0f - 0.1f);
    float mappedSpeed = minSpeed + normalized * (maxSpeed - minSpeed);

    return mappedSpeed;
}

int generatePoisson(float lambda)
{
    int   count = 0;
    float L     = std::exp(-lambda); // Calcul de exp(-lambda)
    float p     = 1.0f;              // Probabilité de l'événement
    do
    {
        p *= random(20); // Tire un nombre aléatoire uniformément sur [0, 1)
        count++;         // Incrémente le nombre d'événements
    } while (p > L); // Continue tant que la probabilité est supérieure à L

    return count - 1; // Soustrait 1 pour obtenir le nombre d'événements
}
