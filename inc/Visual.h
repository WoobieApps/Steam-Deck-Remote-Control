#pragma once

#include <string>
#include <stdexcept>
#include <optional>
#include <map>
#include "SDL.h"
#include "LogWrapper.h"

namespace Visual
{
    constexpr int kScreenWidth = 1000;
    constexpr int kScreenHeight = 800;
    constexpr std::string_view kWindowTitleSV = "Steam Deck Remote Control";
    constexpr uint8_t kWindowInitTrials = 5U;
    constexpr uint8_t kRendererAllocTrials = 5U;
    constexpr uint8_t kTextureLoadTrials = 5U;

    typedef uint32_t textureID_t;

    class TextureWrapper
    {
        private:
            static uint32_t idCounter;
            SDL_Texture* mTexture = nullptr;
            const int mTextureID = ++(Visual::TextureWrapper::idCounter);
            int mWidth = 0;
            int mHeight = 0;

        public:
            TextureWrapper(SDL_Texture*);
            TextureWrapper(TextureWrapper&&) noexcept;
            TextureWrapper(const TextureWrapper&);
            ~TextureWrapper();
            // Used when destroyed or when changing Texture type (for ex. PNG to TTF)
            void freeTexture();
            inline int getWidth() const {return mWidth;}
            inline int getHeight() const {return mHeight;}
            inline textureID_t getID() const {return mTextureID;}
            bool render(SDL_Renderer*, int, int, double rotation = 0.0, SDL_Rect* pClip = nullptr, \
                SDL_RendererFlip flip = SDL_FLIP_NONE, SDL_Rect* pViewportRect = nullptr);
    };

    typedef std::map<textureID_t, TextureWrapper> textureCollection_t;

    class VisualMgr
    {
        private:
            SDL_Renderer* mRenderer = nullptr;
            SDL_Window* mWindow = nullptr;
            textureCollection_t mLoadedTextures;

        public:
            VisualMgr();
            ~VisualMgr();
            bool loadTextureFromPng(std::string);
            const TextureWrapper& getTextureFromID(textureID_t id) const;
            bool renderTexture(TextureWrapper, int, int, double rotation = 0.0, \
                SDL_Rect* pClip = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE, SDL_Rect* pViewportRect = nullptr);
            bool renderTexture(textureID_t, int, int, double rotation = 0.0, \
                SDL_Rect* pClip = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE, SDL_Rect* pViewportRect = nullptr);
    };

}