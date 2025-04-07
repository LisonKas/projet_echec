// maths.hpp
#pragma once

#include <imgui.h>

// Retourne un float pseudo-aléatoire dans [0,1) à partir d'une précision binaire
float random(int precision);

// Loi de Bernoulli avec probabilité p ∈ [0,1]
bool bernoulli(float p, int precision = 20);

// Génère une valeur suivant une loi normale centrée sur 0.1 pour les couleurs sombres
float generateGaussian();

// Approximation d’une loi normale standard sur [0,1], avec un nombre de tirages ajustable
float gaussian_approx(int n = 6);

// Génère une couleur sombre et nuancée (utile pour l’échiquier)
ImVec4 generateFancyDarkColor();

// Tirage d’un entier ∈ {0,1,2,3} selon une loi exponentielle bornée
int tirageExponentialIndex(float lambda = 0.7675f);
