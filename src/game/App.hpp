#pragma once
#include "Chessboard.hpp"

class App {
    private :
        // Renderer3D m_renderer;
        Chessboard m_chessboard;

    public:
        void InitializeGame();
        void StartGame();
        void Update();
        void CloseGame();
};