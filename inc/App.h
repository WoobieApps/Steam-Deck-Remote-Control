#pragma once
#include <optional>
#include "App.h"
#include "VisualMgr.h"

class App
{
    private:
        std::optional<VisualMgr::Window> mWindow;

    public:
        App();
        ~App();
};