#include <imgui.h>
#include <iostream>
#include "3D/Camera.hpp"
#include "game/App.hpp"
#include "quick_imgui/quick_imgui.hpp"

int main()
{
    std::srand(static_cast<unsigned int>(std::time(0)));

    float value{0.f};
    App   app;

    quick_imgui::loop(
        "Quick ImGui",
        {
            .init                     = [&]() { app.InitializeGame(); },
            .loop                     = [&]() { app.StartGame(); },
            .key_callback             = Camera::key_callback, //[](int key, int scancode, int action, int mods) { std::cout << "Key: " << key << " Scancode: " << scancode << " Action: " << action << " Mods: " << mods << '\n'; },
            .mouse_button_callback    = [](int button, int action, int mods) { std::cout << "Button: " << button << " Action: " << action << " Mods: " << mods << '\n'; },
            .cursor_position_callback = [](double xpos, double ypos) { /*std::cout << "Position: " << xpos << ' ' << ypos << '\n';*/ },
            .scroll_callback          = [](double xoffset, double yoffset) { std::cout << "Scroll: " << xoffset << ' ' << yoffset << '\n'; },
            .window_size_callback     = [](int width, int height) { std::cout << "Resized: " << width << ' ' << height << '\n'; },
        }
    );

    app.CloseGame();
}