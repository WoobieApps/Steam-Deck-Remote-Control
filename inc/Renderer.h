#pragma once

#include "SDL.h"
#include "AppLogger.h"
#include <exception>

class Renderer
{
    private:
        SDL_Renderer* mRenderer = NULL;

    public:
        Renderer(SDL_Window* pWindow);
        ~Renderer();

        bool renderTexture(SDL_Texture* pTexture, int position_x, int position_y, double rotation = 0.0, \
            SDL_Rect* pClip = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);
};