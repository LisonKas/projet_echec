#include "App.hpp"
#include <imgui.h>
#include <iostream>
#include "quick_imgui/quick_imgui.hpp"

void App::InitializeGame(){
    m_chessboard.InitializeBoardList();
    m_renderer.initialize();
}

void App::StartGame() {
    ImGui::Begin("Game");
        m_chessboard.CreateBoard();
    ImGui::End();
    ImGui::Begin("3DRenderer");
        m_renderer.render();
    ImGui::End();
}