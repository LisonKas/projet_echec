#pragma once

#include <imgui.h>
#include <iostream>

float random(int precision);

bool bernoulli(float p = 0.4f, int precision = 20);

float gaussian(int n = 6);

float exponential(float lambda = 0.7675f);

int select_index_uniform(int nb_choices);

float logNormalMapped(float mu, float sigma, float minSpeed, float maxSpeed, int precision = 20);

int generatePoisson(float lambda);