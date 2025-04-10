#include "maths.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
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

// Loi de Bernouilli pour désigner le 1er joueur
bool bernoulli(float p, int precision)
{
    float r = random(precision);
    return r < p;
}

// Loi uniforme pour choisir la skybox
int select_index_uniform(int nb_choices)
{
    std::srand(static_cast<unsigned int>(std::time(0)));
    float r = random(20);
    for (int i = 0; i < nb_choices; ++i)
    {
        if (r < static_cast<float>(i + 1) / nb_choices)
        {
            return i;
        }
    }
    return nb_choices - 1;
}

// Loi log-normale pour la vitesse des animations de déplacement
float logNormalMapped(float mu, float sigma, float minSpeed, float maxSpeed, int precision)
{
    std::srand(static_cast<unsigned int>(std::time(0)));

    float u1 = random(precision);
    float u2 = random(precision);

    float z = std::sqrt(-2.0f * std::log(u1)) * std::cos(2 * M_PI * u2);

    float raw = std::exp(mu + sigma * z);

    float clamped = std::clamp(raw, 0.1f, 3.0f); // bornes raisonnables pour la vitesse brute

    float normalized  = (clamped - 0.1f) / (3.0f - 0.1f);
    float mappedSpeed = minSpeed + normalized * (maxSpeed - minSpeed);

    return mappedSpeed;
}

// Loi de Poisson pour le choix des bruitages
int generatePoisson(float lambda)
{
    int   count = 0;
    float L     = std::exp(-lambda);
    float p     = 1.0f;
    do
    {
        p *= random(20);
        count++;
    } while (p > L);

    return count - 1;
}

// Loi de Cauchy pour changer la couleur des lumières
float cauchy(float x0, float gamma, int precision)
{
    float u = random(precision); // dans (0,1)
    return x0 + gamma * std::tan(M_PI * (u - 0.5f));
}

// Loi géométrique pour choisir une ligne ou une colonne à massacrer
int geometric(float p, int max_trials, int precision)
{
    int k = 1;
    while (k < max_trials)
    {
        float r = random(precision);
        if (r < p)
            break;
        ++k;
    }
    return std::min(k - 1, max_trials - 1); // index dans [0, max_trials - 1]
}

// Chaîne de Markov pour changer la musique de fond
enum class Music { Chill,
                   Suspense,
                   Battle };

Music nextMusic(Music current, int precision)
{
    float r = random(precision);

    switch (current)
    {
    case Music::Chill:
        return (r < 0.7f) ? Music::Chill : (r < 0.9f) ? Music::Suspense
                                                      : Music::Battle;
    case Music::Suspense:
        return (r < 0.3f) ? Music::Chill : (r < 0.7f) ? Music::Suspense
                                                      : Music::Battle;
    case Music::Battle:
        return (r < 0.1f) ? Music::Chill : (r < 0.4f) ? Music::Suspense
                                                      : Music::Battle;
    }

    return Music::Chill;
}
