#include "Window.h"

Window::Window()
{
    // Create a window
    mWindow = SDL_CreateWindow(kWindowTitleSV.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, kScreenWidth, kScreenHeight, SDL_WINDOW_SHOWN);
    if(mWindow == nullptr)
    {
        LOG_SDL_ERR("Window not initialized");
        throw std::runtime_error("Window init error");
    }

    // Create a renderer for the window
    try
    {
        mRenderer = Renderer(mWindow);
    }
    catch(const std::exception& exception)
    {
        LOG_SDL_ERR("Renderer not initialized");
        throw std::runtime_error("Renderer init error");
    }
}

Window::~Window()
{
    SDL_DestroyWindow(mWindow);
    mWindow = nullptr;
}