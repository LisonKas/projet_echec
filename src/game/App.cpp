#include "App.hpp"
#include <imgui.h>
#include <iostream>
#include "quick_imgui/quick_imgui.hpp"

void App::StartGame(){
    ImGui::Begin("Game");
    m_chessboard.CreateBoard();
    ImGui::End();
}