#pragma once

#include <string>
#include <stdexcept>
#include <optional>
#include "SDL.h"
#include "LogWrapper.h"

namespace VisualMgr
{
    constexpr int kScreenWidth = 1000;
    constexpr int kScreenHeight = 800;
    constexpr std::string_view kWindowTitleSV = "Steam Deck Remote Control";
    constexpr uint8_t kWindowInitTrials = 5U;
    constexpr uint8_t kRendererAllocTrials = 5U;

    // Wrapper class for SDL_Renderer, implements the render engine of the app
    class Renderer
    {
        private:
            SDL_Renderer* mRenderer = NULL;

        public:
            Renderer(SDL_Window* pWindow);
            ~Renderer();

            bool renderTexture(SDL_Texture* pTexture, int position_x, int position_y, double rotation = 0.0, \
                SDL_Rect* pClip = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE, SDL_Rect* pViewportRect = nullptr);
    };

    // Wrapper class for SDL_Window, implements window management of the app
    class Window
    {
        private:
            SDL_Window* mWindow = NULL;
            std::optional<Renderer> mRenderer;

        public:
            Window();
            ~Window();
    };
}