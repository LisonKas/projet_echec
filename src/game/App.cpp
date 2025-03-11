#include "App.hpp"
#include <imgui.h>
#include <iostream>
#include "quick_imgui/quick_imgui.hpp"

void App::InitializeGame(){
    m_chessboard.InitializeBoardList();
}

void App::StartGame() {
    ImGui::Begin("Game");
        m_chessboard.CreateBoard();
    ImGui::End();
    ImGui::Begin("3DRenderer");
    ImGui::End();
}