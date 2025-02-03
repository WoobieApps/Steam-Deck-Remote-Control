#pragma once

#include "Visual.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "LogWrapper.h"
#include <optional>

namespace App
{
    constexpr int kScreenWidth = 1000;  // To be changed when fullscreen is implemented
    constexpr int kScreenHeight = 800;  // To be changed when fullscreen is implemented
    constexpr std::string_view kWindowTitleSV = "Steam Deck Remote Control";
    class Application
    {
        private:
            std::optional<Visual::VisualMgr> mVisMgr; // Visual manager instance
            bool mQuit;         // Flag to quit the application
            void initApp();     // Helper function to initialize App dependencies
            void closeApp();    // Helper function to close App dependencies

        public:
            Application();
            ~Application();
            void run();
    };
}
