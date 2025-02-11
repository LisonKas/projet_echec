#pragma once
#include <utility>

struct Square {
    int m_id;
    bool m_color_light;
    bool m_is_occupied;
    std::pair<char, int> m_coords;
};