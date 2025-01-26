#include <format>
#include <stdexcept>
#include "VisualMgr.h"

    VisualMgr::Renderer::Renderer(SDL_Window* pWindow)
    {
        uint8_t initTrials = 0U;
        do
        {
            if(initTrials >= kRendererAllocTrials)
            {
                // Throw since we failed to initialize the renderer after 'kRenderAllocTrials' tries
                throw std::runtime_error("Renderer alloc error");
            }
            mRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
            initTrials++;
        }
        while(mRenderer == nullptr);
        LOG_INF("Renderer: Renderer initialized successfully.");
    }

    VisualMgr::Renderer::~Renderer()
    {
        SDL_DestroyRenderer(mRenderer);
        mRenderer = nullptr;
        LOG_INF("Renderer: Renderer destroyed successfully.");
    }

    bool VisualMgr::Renderer::renderTexture(SDL_Texture* pTexture, int position_x, int position_y, double rotation, \
        SDL_Rect* pClip, SDL_RendererFlip flip, SDL_Rect* pViewportRect)
    {
        // We need to query the texture for its fields, since SDL implements SDL_Texture as incomplete type.
        int textureWidth, textureHeight;
        if(SDL_QueryTexture(pTexture, nullptr, nullptr, &textureWidth, &textureHeight) != 0)
        {
            LOG_SDL_ERR("Renderer: Could not get Texture query.");
            return false;
        }
        SDL_Rect renderRect = {position_x, position_y, textureWidth, textureHeight};

        // Handle viewport if applicable
        if(pViewportRect == nullptr)
        {
            SDL_RenderSetViewport(mRenderer, pViewportRect);
        }

        // Handle clipping if applicable
        if(pClip != nullptr)
        {
            renderRect.w = pClip->w;
            renderRect.h = pClip->h;
        }

        // Render final texture
        SDL_RenderCopyEx(mRenderer, pTexture, pClip, &renderRect, rotation, nullptr, flip);
        return true;
    }

    VisualMgr::Window::Window()
    {
        try
        {
            // Create a window
            uint8_t initTrials = 0U;
            do
            {
                if(initTrials >= kWindowInitTrials)
                {
                    // Throw since we could not initialize Window after 'kWindowInitTrials' tries.
                    throw std::runtime_error("SDL_Window alloc error");
                }
                mWindow = SDL_CreateWindow(kWindowTitleSV.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, kScreenWidth, kScreenHeight, SDL_WINDOW_SHOWN);
                initTrials++;
            } while(mWindow == nullptr);

            // Create a renderer for the window
            mRenderer.emplace(mWindow);
            LOG_INF("Window: Window created successfully.");
        }
        catch(const std::exception& exception)
        {
            // Re-throw an exception, no sense in having a window with no render engine
            // Important, deallocate the window as ~Window will not be called here
            SDL_DestroyWindow(mWindow);
            mWindow = nullptr;
            throw std::runtime_error(std::format("Window init error: {}", exception.what()));
        }
    }

    VisualMgr::Window::~Window()
    {
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
        LOG_INF("Window: Window destroyed successfully.");
    }