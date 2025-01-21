#pragma once
#include "Chessboard.hpp"

class App {
    private :
        //3D_RENDERER m_renderer;
        //SOUND_MANAGER m_sound;
        Chessboard m_chessboard;

    public:
        void StartGame();
        void Update();
        void CloseGame();
};