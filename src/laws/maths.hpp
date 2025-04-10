// maths.hpp
#pragma once

#include <imgui.h>
#include <iostream>

// Retourne un float pseudo-aléatoire dans [0,1) à partir d'une précision binaire
float random(int precision);

// Loi de Bernoulli avec probabilité p ∈ [0,1]
bool bernoulli(float p, int precision = 20);

float gaussian(int n = 6);

float exponential(float lambda = 0.7675f);

// Génère une couleur sombre et nuancée (utile pour l’échiquier)
ImVec4 generateFancyDarkColor();

// Génère un float pseudo-aléatoire ∈ [0,1) (uniforme) avec une précision donnée
float random_uniform(int precision = 20);

// Tirage uniforme d’un index dans [0, nb_choices-1] à partir de random_uniform
int select_index_uniform(int nb_choices);

float logNormalMapped(float mu, float sigma, float minSpeed, float maxSpeed, int precision = 20);

int generatePoisson(float lambda);