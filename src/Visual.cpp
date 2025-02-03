#include "Visual.h"
#include "SDL_image.h"
#include <format>

// This constructor may throw an exception if the texture is not loaded properly.
Visual::TextureWrapper::TextureWrapper(Visual::RendererPtr& renderer, const std::string& path):
    mTexture(nullptr, SDL_DestroyTexture),
    mName([](const std::string& path) -> const std::string  // Lambda to extract the name of the texture from the path
    {
        size_t pos = path.find_last_of("/\\");
        if (pos != std::string::npos)
        {
            return path.substr(pos + 1);
        }
        return path;
    } (path)),
    mWidth(Visual::kInitTextureWidth),
    mHeight(Visual::kInitTextureHeight)
{
    SDL_Texture* texture = IMG_LoadTexture(renderer.get(), path.c_str());   // Load the texture
    if(texture == nullptr)
    {
        THROW_EXCEPTION("Error when loading a texture");   // Throw an exception if the texture is not loaded
    }
    mTexture.reset(texture);    // Assign the texture to the unique pointer
    LOG_INF(std::format("Texture with name: {} loaded successfully.", mName));
}

Visual::TextureWrapper::~TextureWrapper()
{
    LOG_INF(std::format("Texture with name: {} destroyed successfully.", mName));
}

// Render the texture with the given Renderer and other parameters
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
    // Create a rectangle where we want to render the texture
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

// This constructor may throw an exception if the window or renderer is not created properly.
Visual::VisualMgr::VisualMgr(std::string_view window_title, int windowWidth, int windowHeight):
    mWindowWidth(windowWidth),
    mWindowHeight(windowHeight),
    mWindowTitle(window_title),
    mWindow(nullptr, SDL_DestroyWindow),
    mRenderer(nullptr, SDL_DestroyRenderer)
{
    uint8_t initTrials = 0U;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    // Create a window
    while(initTrials < Visual::kWindowInitTrials)
    {
        window = SDL_CreateWindow(mWindowTitle.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, \
            mWindowWidth, mWindowHeight, SDL_WINDOW_SHOWN);
        initTrials++;
        if(window != nullptr) break;
    }
    // If the window is not created properly after kWindowInitTrials, throw an exception
    if(initTrials >= Visual::kWindowInitTrials) THROW_EXCEPTION("SDL_Window alloc error");

    // Create a renderer for the window
    initTrials = 0U;
    while(initTrials < Visual::kRendererAllocTrials)
    {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        initTrials++;
        if(renderer != nullptr) break;
    }
    // If the renderer is not created properly after kRendererAllocTrials, throw an exception
    if(initTrials >= Visual::kRendererAllocTrials)
    {
        // Remember to destroy the window before throwing an exception
        SDL_DestroyWindow(window);
        THROW_EXCEPTION("SDL_Renderer alloc error");
    }
    // Assign the window and renderer to the unique pointers
    mWindow.reset(window);
    LOG_INF("Window initialized successfully.");
    mRenderer.reset(renderer);
    LOG_INF("Renderer Initialized successfully.");
}

Visual::VisualMgr::~VisualMgr()
{
    // Destroy the Textures first
    mLoadedTextures.clear();
    LOG_INF("Destroyed successfully.");
}

// Load a texture from a PNG file in given path
bool Visual::VisualMgr::loadTextureFromPng(const std::string& path)
{
    try
    {
        std::unique_ptr<Visual::TextureWrapper> newTW = std::make_unique<Visual::TextureWrapper>(mRenderer, path);
        mLoadedTextures[newTW.get()->getName()] = std::move(newTW);
        return true;
    }
    catch(const std::exception& exception)
    {
        // Texture was not loaded properly
        LOG_SDL_ERR(exception.what());
        return false;
    }
}

// Render a texture with the given name in the texture collection
bool Visual::VisualMgr::renderTexture(const std::string& texture_name, int position_x, int position_y, double rotation, \
        SDL_Rect* pClip, SDL_RendererFlip flip, SDL_Rect* pViewportRect)
{
    auto foundTextureIT = mLoadedTextures.find(texture_name);
    if(foundTextureIT == mLoadedTextures.end())
    {
        LOG_SDL_ERR(std::format("Texture of name: {} was not found in loaded textures!",texture_name));
        return false;
    }
    if(!(foundTextureIT->second->render(mRenderer, position_x, position_y, rotation, pClip, flip, pViewportRect)))
    {
        LOG_SDL_ERR(std::format("Texture of name: {} render failed.",texture_name));
        return false;
    }
    else
    {
        // Do not log here, it will clutter the buffer
        return true;
    }
}