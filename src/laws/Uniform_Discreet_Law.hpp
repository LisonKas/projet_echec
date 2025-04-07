#pragma once

#ifndef UNIFORM_DISCREET_LAW_HPP
#define UNIFORM_DISCREET_LAW_HPP

#include <iostream>
#include <vector>

class UniformDiscreetLaw {
private:
    unsigned long long seed;
    static const unsigned long long a = 1664525;
    static const unsigned long long c = 1013904223;
    static const unsigned long long m = 4294967296; 

public:
    UniformDiscreetLaw(unsigned long long seed);

    unsigned int next();
    int uniform_int(int min, int max);

    unsigned int operator()();
    template <typename T>
    void custom_shuffle(std::vector<T>& vec);
};

int simulate_uniform_discrete(int seed);

#endif