#include "Chessboard.hpp"
#include <imgui.h>
#include <iostream>
#include <vector>
#include "quick_imgui/quick_imgui.hpp"

void Chessboard::InitializeBoardList(){
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
            line_squares.push_back(new_square);
        }
        this->m_boardlist.push_back(line_squares);
    }
}

void Chessboard::CreateBoard(){
    this->InitializeBoardList();
    int id {0};
    for(int i {0}; i<8; i++){
        for(int j {0}; j<8; j++){
            if(this->m_boardlist[i][j].m_color_light){
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                // if(i==0) name = '0';else name = 'i';
            }
            else {
                // if(i==0) name = '0';else name = 'i';
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
            }
            ImGui::PushID(id);
            if (ImGui::Button(" ", ImVec2{100.f, 100.f}))
                    std::cout << "Clicked button : " << i << " and " << j << std::endl;
            ImGui::PopID();
            ImGui::PopStyleColor();
            id++;

            if (j < 7) {
                ImGui::SameLine(); // Rester sur la même ligne pour les colonnes
            }
        }
    }
}