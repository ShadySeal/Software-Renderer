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
        Scene& scene;
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        SDL_Texture* texture = nullptr;
        std::unique_ptr<Canvas> canvas;

        const int wWidth;
        const int wHeight;
        const int wScale;
        bool initialized = false;

        bool init();
        bool logSdlError(const char* message);
        void cleanup();

    public:
        Renderer(Scene& scene, int cW, int cH, int wS = 1);
        ~Renderer();

        void render();
    };
}
