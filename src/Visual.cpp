#include <format>
#include <stdexcept>
#include "Visual.h"
#include "SDL_image.h"

Visual::TextureWrapper::TextureWrapper(Visual::RendererPtr& renderer, const std::string& path):
    mTexture(nullptr, SDL_DestroyTexture),
    mPath(path),
    mWidth(Visual::kInitTextureWidth),
    mHeight(Visual::kInitTextureHeight)
{
    SDL_Texture* texture = IMG_LoadTexture(renderer.get(), path.c_str());
    if(texture == nullptr)
    {
        THROW_EXCEPTION("Error when loading a texture");
    }
    mTexture.reset(texture);
    LOG_INF(std::format("Texture for path {} loaded successfully.", mPath));
}

Visual::TextureWrapper::~TextureWrapper()
{
    LOG_INF(std::format("Texture for path {} destroyed successfully.", mPath));
}

bool Visual::TextureWrapper::render(Visual::RendererPtr& renderer, int position_x, int position_y, double rotation, SDL_Rect* pClip, \
    SDL_RendererFlip flip, SDL_Rect* pViewportRect)
{
    // We need to query the texture for its fields, since SDL implements SDL_Texture as incomplete type.
    int textureWidth, textureHeight;
    if(SDL_QueryTexture(mTexture.get(), nullptr, nullptr, &textureWidth, &textureHeight) != 0)
    {
        LOG_SDL_ERR("Could not get Texture query for render purpose.");
        return false;
    }
    SDL_Rect renderRect = {position_x, position_y, textureWidth, textureHeight};

    // Handle viewport if applicable
    if(pViewportRect == nullptr)
    {
        SDL_RenderSetViewport(renderer.get(), pViewportRect);
    }

    // Handle clipping if applicable
    if(pClip != nullptr)
    {
        renderRect.w = pClip->w;
        renderRect.h = pClip->h;
    }

    // Render final texture
    SDL_RenderCopyEx(renderer.get(), mTexture.get(), pClip, &renderRect, rotation, nullptr, flip);
    return true;
}

Visual::TextureID Visual::VisualMgr::mTextureCounter = 0U;

Visual::VisualMgr::VisualMgr():
    mWindow(nullptr, SDL_DestroyWindow),
    mRenderer(nullptr, SDL_DestroyRenderer)
{
    uint8_t initTrials = 0U;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    // Create a window
    while(initTrials < Visual::kWindowInitTrials)
    {
        window = SDL_CreateWindow(Visual::kWindowTitleSV.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, \
            Visual::kScreenWidth, Visual::kScreenHeight, SDL_WINDOW_SHOWN);
        initTrials++;
        if(window != nullptr) break;
    }
    if(initTrials >= Visual::kWindowInitTrials) THROW_EXCEPTION("SDL_Window alloc error");
    mWindow.reset(window);
    LOG_INF("Window initialized successfully.");

    // Create a renderer for the window
    initTrials = 0U;
    while(initTrials < Visual::kRendererAllocTrials)
    {
        renderer = SDL_CreateRenderer(mWindow.get(), -1, SDL_RENDERER_ACCELERATED);
        initTrials++;
        if(renderer != nullptr) break;
    }
    if(initTrials >= Visual::kRendererAllocTrials) THROW_EXCEPTION("SDL_Renderer alloc error");
    mRenderer.reset(renderer);
    LOG_INF("Renderer Initialized successfully.");
}

Visual::VisualMgr::~VisualMgr()
{
    LOG_INF("Destroyed successfully.");
}

bool Visual::VisualMgr::loadTextureFromPng(const std::string& path)
{
    try
    {
        std::unique_ptr<Visual::TextureWrapper> newTW = std::make_unique<Visual::TextureWrapper>(mRenderer, path);
        mLoadedTextures[mTextureCounter] = std::move(newTW);
        return true;
    }
    catch(const std::exception& exception)
    {
        LOG_SDL_ERR(exception.what());
        return false;
    }
}

// const Visual::TextureWrapper& Visual::VisualMgr::getTextureFromID(textureID_t id) const
// {
//     auto it = mLoadedTextures.find(id);
//     if(it == mLoadedTextures.end())
//     {
//         throw std::runtime_error(std::format("Texture ID {} not found.", id));
//     }
//     return it->second;
// }

// bool Visual::VisualMgr::renderTexture(TextureWrapper textureWrap, int position_x, int position_y, double rotation, \
//         SDL_Rect* pClip, SDL_RendererFlip flip, SDL_Rect* pViewportRect)
// {
//     return textureWrap.render(mRenderer, position_x, position_y, rotation, pClip, flip, pViewportRect);
// }

// bool Visual::VisualMgr::renderTexture(Visual::textureID_t textID, int position_x, int position_y, double rotation, \
//         SDL_Rect* pClip, SDL_RendererFlip flip, SDL_Rect* pViewportRect)
// {
//     Visual::TextureWrapper textureWrap = getTextureFromID(textID);
//     return textureWrap.render(mRenderer, position_x, position_y, rotation, pClip, flip, pViewportRect);
// }