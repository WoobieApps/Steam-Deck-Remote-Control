#pragma once
#include <optional>
#include "App.h"
#include "Visual.h"

class App
{
    private:
        // Using std::optional to delay the initialization since Window() can throw, we need to be able to handle that
        std::optional<Visual::VisualMgr> mVisMgr;

    public:
        App();
        ~App();
        void run();
};