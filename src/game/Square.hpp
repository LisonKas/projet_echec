#pragma once
#include <utility>

struct Square {
    bool m_color_light;
    bool m_is_occupied;
    std::pair<char, int> coords;
};