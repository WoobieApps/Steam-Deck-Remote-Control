#include "App.h"
#include <stdexcept>
#include <format>

App::Application::Application():
    mQuit(false)
{
    try
    {
        initApp();
        mVisMgr.emplace(App::kWindowTitleSV, App::kScreenWidth, App::kScreenHeight); // Initialize Visual Manager
        LOG_INF("Application initialized successfully.");
    }
    catch(const std::exception& exception)
    {
        // Re-throw main App-level exception and let the executable fail.
        closeApp();
        throw std::runtime_error(std::format("App init error: {}", exception.what()));
    }
}

App::Application::~Application()
{
    mVisMgr.reset();    // Destroy Visual Manager
    closeApp();
    LOG_INF("Destroyed successfully.");
}

void App::Application::initApp()
{
    // Initialize Simple DirectMedia Layer
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
        THROW_EXCEPTION("Could not initialize SDL.");
    }

    // Initialize SDL Image library
    int imgFlags = IMG_INIT_PNG;
    if	( !( IMG_Init( imgFlags ) & imgFlags ) )	{
        SDL_Quit();
        THROW_EXCEPTION("SDL_image could not be initialized.");
    }

    // Initialize True Type Fonts for SDL
    if (TTF_Init() != 0)
    {
        IMG_Quit();
        SDL_Quit();
        THROW_EXCEPTION("True Type Fonts could not be initialized.");
    }
    LOG_INF("Main: Dependencies initialized successfully.");
}

void App::Application::closeApp()
{
    TTF_Quit(); // Close True Type Fonts
    SDL_Quit(); // Close Simple DirectMedia Layer
    LOG_INF("Main: Dependencies closed successfully.");
}

void App::Application::run()
{
    // TODO: this is a test
    bool quit = false;
    mVisMgr->loadTextureFromPng("./media/example.png");
    SDL_Event e;
    while( !quit )
    {
        // Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
        }
        mVisMgr->callRenderClear();
        mVisMgr->renderTexture("example.png",0,0);
        mVisMgr->renderTexture("example.png",500,400);
        mVisMgr->callRenderPresent();
    }
}