#pragma once

#include "SDL.h"
#include "LogWrapper.h"
#include <string>
#include <stdexcept>
#include <optional>
#include <map>
#include <memory>

namespace Visual
{
    constexpr int kScreenWidth = 1000;
    constexpr int kScreenHeight = 800;
    constexpr std::string_view kWindowTitleSV = "Steam Deck Remote Control";
    constexpr uint8_t kWindowInitTrials = 5U;
    constexpr uint8_t kRendererAllocTrials = 5U;
    constexpr uint8_t kTextureLoadTrials = 5U;
    constexpr int kInitTextureWidth = 0;
    constexpr int kInitTextureHeight = 0;

    using TexturePtr = std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)>;
    using RendererPtr = std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>;
    using WindowPtr = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;
    using TextureID = uint32_t;

    class TextureWrapper
    {
        private:
            TexturePtr mTexture;
            int mWidth;
            int mHeight;

        public:
            TextureWrapper(RendererPtr, const std::string&);
            ~TextureWrapper();
            inline int getWidth() const {return mWidth;}
            inline int getHeight() const {return mHeight;}
            bool render(RendererPtr, int, int, double rotation = 0.0, SDL_Rect* pClip = nullptr, \
                SDL_RendererFlip flip = SDL_FLIP_NONE, SDL_Rect* pViewportRect = nullptr);
    };

    using TextureCollection = std::map<TextureID, TextureWrapper>;

    class VisualMgr
    {
        private:
            RendererPtr mRenderer;
            WindowPtr mWindow;
            TextureCollection mLoadedTextures;

        public:
            VisualMgr();
            ~VisualMgr();
            bool loadTextureFromPng(std::string);
            bool renderTexture(TextureWrapper, int, int, double rotation = 0.0, \
                SDL_Rect* pClip = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE, SDL_Rect* pViewportRect = nullptr);
    };

}