#include <stdexcept>
#include <format>
#include "App.h"

App::App()
{
    try
    {
        mVisMgr.emplace();
        LOG_INF("App: Application initialized successfully.");
    }
    catch(const std::exception& exception)
    {
        // Re-throw main App-level exception and let the executable fail.
        // There is no sense in using the app when any of the components fail to initialize.
        throw std::runtime_error(std::format("App init error: {}", exception.what()));
    }
}

App::~App()
{
    LOG_INF("App: Destroyed successfully.");
}

void App::run()
{
    mVisMgr->loadTextureFromPng("./media/example.png");
    mVisMgr->
}