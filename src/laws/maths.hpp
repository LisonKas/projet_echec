// maths.hpp
#pragma once

#include <imgui.h>

// Retourne un float pseudo-aléatoire dans [0,1) à partir d'une précision binaire
float random(int precision);

// Génère une couleur grise sombre suivant une loi normale centrée sur 0.1
float generateGaussian();

// Approximation d’une loi normale standard sur [0,1], avec un nombre de tirages ajustable
float gaussian_approx(int n = 6);

// Génère une couleur sombre et nuancée (pour l’échiquier)
ImVec4 generateFancyDarkColor();
