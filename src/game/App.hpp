#pragma once
#include "Chessboard.hpp"
#include "../3D/Renderer3D.hpp"

class App {
    private :
        Renderer3D m_renderer;
        Chessboard m_chessboard;

    public:
        void InitializeGame();
        void StartGame();
        void Update();
        void CloseGame();
};