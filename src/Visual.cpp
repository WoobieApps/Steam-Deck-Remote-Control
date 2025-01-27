#include <format>
#include <stdexcept>
#include "Visual.h"
#include "SDL_image.h"

uint32_t Visual::TextureWrapper::idCounter = 0U;

Visual::TextureWrapper::TextureWrapper(SDL_Texture* texture)
{
    mTexture = texture;
    LOG_INF("TextureWrapper: Initialized successfully.");
}

Visual::TextureWrapper::TextureWrapper(TextureWrapper&& other) noexcept
{
    mTexture = other.mTexture;
    mWidth = other.mWidth;
    mHeight = other.mHeight;
}

Visual::TextureWrapper::~TextureWrapper()
{
    freeTexture();
    LOG_INF("TextureWrapper: Destroyed successfully.");
}

void Visual::TextureWrapper::freeTexture()
{
    if(mTexture != nullptr)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
    }
}

bool Visual::TextureWrapper::render(SDL_Renderer* pRenderer, int position_x, int position_y, double rotation, SDL_Rect* pClip, \
    SDL_RendererFlip flip, SDL_Rect* pViewportRect)
{
    // We need to query the texture for its fields, since SDL implements SDL_Texture as incomplete type.
    int textureWidth, textureHeight;
    if(SDL_QueryTexture(mTexture, nullptr, nullptr, &textureWidth, &textureHeight) != 0)
    {
        LOG_SDL_ERR("Renderer: Could not get Texture query.");
        return false;
    }
    SDL_Rect renderRect = {position_x, position_y, textureWidth, textureHeight};

    // Handle viewport if applicable
    if(pViewportRect == nullptr)
    {
        SDL_RenderSetViewport(pRenderer, pViewportRect);
    }

    // Handle clipping if applicable
    if(pClip != nullptr)
    {
        renderRect.w = pClip->w;
        renderRect.h = pClip->h;
    }

    // Render final texture
    SDL_RenderCopyEx(pRenderer, mTexture, pClip, &renderRect, rotation, nullptr, flip);
    return true;
}

Visual::VisualMgr::VisualMgr()
{
    // Create a window
    uint8_t initTrials = 0U;
    do
    {
        if(initTrials >= Visual::kWindowInitTrials)
        {
            // Throw since we could not initialize Window after 'kWindowInitTrials' tries.
            throw std::runtime_error("SDL_Window alloc error");
        }
        mWindow = SDL_CreateWindow(Visual::kWindowTitleSV.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, \
            Visual::kScreenWidth, Visual::kScreenHeight, SDL_WINDOW_SHOWN);
        initTrials++;
    } while(mWindow == nullptr);
    LOG_INF("Window: Initialized successfully.");

    // Create a renderer for the window
    initTrials = 0U;
    do
    {
        if(initTrials >= Visual::kRendererAllocTrials)
        {
            // Throw since we failed to initialize the renderer after 'kRenderAllocTrials' tries
            throw std::runtime_error("SDL_Renderer alloc error");
        }
        mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
        initTrials++;
    }
    while(mRenderer == nullptr);
    LOG_INF("Renderer: Initialized successfully.");
}

Visual::VisualMgr::~VisualMgr()
{
    SDL_DestroyRenderer(mRenderer);
    mRenderer = nullptr;
    LOG_INF("Renderer: Destroyed successfully.");
    SDL_DestroyWindow(mWindow);
    mWindow = nullptr;
    LOG_INF("Window: Destroyed successfully.");
}

bool Visual::VisualMgr::loadTextureFromPng(std::string path)
{
    SDL_Texture* texture = IMG_LoadTexture(mRenderer, path.c_str());
    if(texture == nullptr) return false;
    Visual::TextureWrapper newTW = Visual::TextureWrapper(texture);
    mLoadedTextures.insert(std::make_pair(newTW.getID(), newTW));
    return true;
}

const Visual::TextureWrapper& Visual::VisualMgr::getTextureFromID(textureID_t id) const
{
    auto it = mLoadedTextures.find(id);
    if(it == mLoadedTextures.end())
    {
        throw std::runtime_error(std::format("Texture ID {} not found.", id));
    }
    return it->second;
}

bool Visual::VisualMgr::renderTexture(TextureWrapper textureWrap, int position_x, int position_y, double rotation, \
        SDL_Rect* pClip, SDL_RendererFlip flip, SDL_Rect* pViewportRect)
{
    return textureWrap.render(mRenderer, position_x, position_y, rotation, pClip, flip, pViewportRect);
}

bool Visual::VisualMgr::renderTexture(Visual::textureID_t textID, int position_x, int position_y, double rotation, \
        SDL_Rect* pClip, SDL_RendererFlip flip, SDL_Rect* pViewportRect)
{
    Visual::TextureWrapper textureWrap = getTextureFromID(textID);
    return textureWrap.render(mRenderer, position_x, position_y, rotation, pClip, flip, pViewportRect);
}