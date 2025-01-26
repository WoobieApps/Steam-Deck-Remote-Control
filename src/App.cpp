#include <stdexcept>
#include <format>
#include "App.h"

App::App()
{
    try
    {
        mWindow.emplace();
        LOG_INF("App: Application initialized successfully.");
    }
    catch(const std::exception& exception)
    {
        // Throw main App-level exception and let the executable fail, there is no sense in using the app without working window.
        throw std::runtime_error(std::format("App init error: {}", exception.what()));
    }
}

App::~App()
{
    LOG_INF("App: Application destroyed successfully.");
}