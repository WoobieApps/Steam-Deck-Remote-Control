#pragma once

#include <string>
#include <exception>
#include <optional>
#include "SDL_video.h"
#include "AppLogger.h"
#include "Renderer.h"

constexpr int kScreenWidth = 1000;
constexpr int kScreenHeight = 800;
constexpr std::string_view kWindowTitleSV = "Steam Deck Remote Control";

class Window
{
    private:
        SDL_Window* mWindow = NULL;
        std::optional<Renderer> mRenderer;

    public:
        Window();
        ~Window();
};