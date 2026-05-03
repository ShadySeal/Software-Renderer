#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include <memory>
#include <string>
#include "canvas.h"

namespace software_renderer
{
    struct Scene;
    
    class Renderer
    {
    private:
        Scene& scene_;
        SDL_Window* window_ = nullptr;
        SDL_Renderer* renderer_ = nullptr;
        SDL_Texture* texture_ = nullptr;
        std::unique_ptr<Canvas> canvas_;

        const int wWidth_;
        const int wHeight_;
        const int wScale_;
        bool initialized_ = false;

        bool init();
        bool logSdlError(const char* message);
        void cleanup();

    public:
        Renderer(Scene& scene, int cW, int cH, int wS = 1);
        ~Renderer();

        void render();
    };
}
