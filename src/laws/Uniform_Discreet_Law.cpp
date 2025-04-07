#include "Uniform_Discreet_Law.hpp"

UniformDiscreetLaw::UniformDiscreetLaw(unsigned long long seed) : seed(seed) {}

unsigned int UniformDiscreetLaw::next() {
    seed = (a * seed + c) % m;
    return static_cast<unsigned int>(seed);
}

int UniformDiscreetLaw::uniform_int(int min, int max) {
    return min + (next() % (max - min + 1));
}

int simulate_uniform_discrete(int seed) {
    UniformDiscreetLaw rng(seed);
    return rng.uniform_int(0, 959);
}

// unsigned int UniformDiscreetLaw::operator()() {
//     m_state = (m_state * 1664525 + 1013904223) % 4294967296;
//     return m_state;
// }

// template <typename T>
// void UniformDiscreetLaw::custom_shuffle(std::vector<T>& vec) {
//     size_t n = vec.size();
//     for (size_t i = 0; i < n - 1; ++i) {
//         // Sélectionner un indice aléatoire entre i et n-1
//         size_t j = i + (this->operator()() % (n - i)); // L'équivalent de rand() % (n-i)
//         std::swap(vec[i], vec[j]); // Échanger les éléments
//     }
// }