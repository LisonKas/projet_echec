#include "Chessboard.hpp"
#include <imgui.h>
#include <iostream>
#include <vector>
#include "quick_imgui/quick_imgui.hpp"
#include <memory>
#include <utility>

void Chessboard::InitializeBoardList(){
    std::vector<char> j_as_chars = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    for(int i {0}; i<8; i++){
        std::vector<Square> line_squares;
        for(int j {0}; j<8; j++){
            Square new_square;
            if(i%2==0){
                new_square.m_color_light = j%2==0 ? false : true;
            }
            else {
                new_square.m_color_light = j%2==0 ? true : false;
            }
            if(i==0 || i==1 || i==6 || i==7){
                new_square.m_is_occupied = true;
            }
            new_square.m_coords = std::pair<char, int>( j_as_chars[j], 8-i);
            line_squares.push_back(new_square);
        }
        this->m_boardlist.push_back(line_squares);
    }
    m_pieces.InitializeAllPieces();
}

void Chessboard::CreateBoard(){
    int id {0};
    for(int i {0}; i<8; i++){
        for(int j {0}; j<8; j++){
            if(this->m_boardlist[i][j].m_color_light){
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            }
            else {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
            }
            ImGui::PushID(id);

            if (ImGui::Button(" ", ImVec2{100.f, 100.f})) //button_label.c_str()
                    std::cout << "Clicked button : " << this->m_boardlist[i][j].m_coords.first << "" << this->m_boardlist[i][j].m_coords.second << std::endl;
            ImGui::PopID();
            ImGui::PopStyleColor();
            id++;

            if (j < 7) {
                ImGui::SameLine(); 
            }
        }
    }
}



/* Ici c'est pour que les coordonnées apparaissent sur les cases
            char coord_x = this->m_boardlist[i][j].m_coords.first;
            int coord_y = this->m_boardlist[i][j].m_coords.second;

            std::string button_label = "(";
            button_label += coord_x; 
            button_label += ",";
            button_label += std::to_string(coord_y);
            button_label += ")"; */