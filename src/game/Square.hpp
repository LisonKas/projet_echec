#pragma once
#include <imgui.h>
#include <utility>

struct Square {
    int                  m_id;
    bool                 m_color_light;
    bool                 m_is_occupied = false;
    std::pair<char, int> m_coords;
    bool                 isOccupied() const;
    bool                 isOccupied(std::pair<int, int> coords) const;
    ImVec4               m_dark_color; // Valeur par défaut
};