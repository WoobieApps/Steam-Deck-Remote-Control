#pragma once

#include "SDL.h"
#include "LogWrapper.h"
#include <string>
#include <stdexcept>
#include <optional>
#include <map>
#include <memory>

// Namespace where all things visual reside
namespace Visual
{
    constexpr uint8_t kWindowInitTrials = 5U;       // Number of trials of window initalization before throwing an exception
    constexpr uint8_t kRendererAllocTrials = 5U;    // Number of trials of renderer allocation before throwing an exception
    constexpr int kInitTextureWidth = 0;            // Initial texture width
    constexpr int kInitTextureHeight = 0;           // Initial texture height

    using TexturePtr = std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)>;     // Unique pointer to SDL_Texture
    using RendererPtr = std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>;  // Unique pointer to SDL_Renderer
    using WindowPtr = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;        // Unique pointer to SDL_Window

    // Wrapper class for SDL_Texture
    class TextureWrapper
    {
        private:
            TexturePtr mTexture;
            const std::string mName;    // Contains the name and extenstion present in ./media/ folder
            int mWidth;                 // Width of the texture
            int mHeight;                // Height of the texture

        public:
            TextureWrapper(RendererPtr&, const std::string&);
            ~TextureWrapper();
            inline int getWidth() const {return mWidth;}
            inline int getHeight() const {return mHeight;}
            inline const std::string getName() const {return mName;}
            bool render(RendererPtr&, int, int, double rotation = 0.0, SDL_Rect* pClip = nullptr, \
                SDL_RendererFlip flip = SDL_FLIP_NONE, SDL_Rect* pViewportRect = nullptr);  // Render the texture using given render engine
    };

    using TextureCollection = std::map<std::string, std::unique_ptr<TextureWrapper>>;   // Collection of textures mapped by their names

    // Manager class for SDL_Window and SDL_Renderer
    class VisualMgr
    {
        private:
            int mWindowWidth;
            int mWindowHeight;
            const std::string mWindowTitle;     // Title of the window
            TextureCollection mLoadedTextures;  // Collection of loaded textures
            RendererPtr mRenderer;
            WindowPtr mWindow;

        public:
            VisualMgr(std::string_view, int, int);
            ~VisualMgr();
            bool loadTextureFromPng(const std::string& path);                           // Load a texture from a PNG file
            bool renderTexture(const std::string&, int, int, double rotation = 0.0, \
                SDL_Rect* pClip = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE, SDL_Rect* pViewportRect = nullptr); // Render a texture in texture collection
            void callRenderPresent() {SDL_RenderPresent(mRenderer.get());}  // Present a new frame
            void callRenderClear() {SDL_RenderClear(mRenderer.get());}      // Clear the rendered screen
    };

}