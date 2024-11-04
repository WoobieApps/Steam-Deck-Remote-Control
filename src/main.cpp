#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "AppLogger.h"

bool initApp();
void closeApp();

int main(int argc, char* argv[])
{
    initApp();

    closeApp();
    return 0;
}

bool initApp()
{
    // Initialize Simple DirectMedia Layer
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
        LOG_SDL_ERR("Could not initialize SDL.");
        return false;
    }

    // Initialize SDL Image library
    int imgFlags = IMG_INIT_PNG;
    if	( !( IMG_Init( imgFlags ) & imgFlags ) )	{
        LOG_SDL_ERR("SDL_image could not be initialized.");
        SDL_Quit();
        return false;
    }

    // Initialize True Type Fonts for SDL
    if (TTF_Init() != 0)
    {
        LOG_SDL_ERR("True Type Fonts could not be initialized.");
        SDL_Quit();
        return false;
    }
    LOG_INF("Application initialized successfully.");
    return true;
}

void closeApp()
{
    TTF_Quit();
    SDL_Quit();
}