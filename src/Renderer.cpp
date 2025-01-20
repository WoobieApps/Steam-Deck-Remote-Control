#include "Renderer.h"

Renderer::Renderer(SDL_Window* pWindow)
{
    mRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
    if(mRenderer == nullptr)
    {
        LOG_SDL_ERR("Renderer not initialized");
        throw "Renderer init error";
    }
}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(mRenderer);
    mRenderer = nullptr;
}

bool Renderer::renderTexture(SDL_Texture* pTexture, int position_x, int position_y, double rotation, SDL_Rect* pClip, SDL_RendererFlip flip)
{
    int textureWidth, textureHeight;
    if(SDL_QueryTexture(pTexture, nullptr, nullptr, &textureWidth, &textureHeight) != 0)
    {
        LOG_SDL_ERR("Could not get Texture query.");
        return false;
    }

    SDL_Rect renderRect = {position_x, position_y, textureWidth, textureHeight};
    if(pClip != nullptr)
    {
        renderRect.w = pClip->w;
        renderRect.h = pClip->h;
    }

    SDL_RenderCopyEx(mRenderer, pTexture, pClip, &renderRect, rotation, nullptr, flip);

    return true;
}