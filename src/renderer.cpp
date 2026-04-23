#include "renderer.h"
#include "raytracer.h"
#include "scene.h"

using namespace software_renderer;

Renderer::Renderer(Scene& scene, int wWidth, int wHeight, int wScale)
    : scene(scene), wWidth(wWidth), wHeight(wHeight), wScale(wScale)
{
    initialized = init();
}

Renderer::~Renderer()
{
    cleanup();
}

bool Renderer::logSdlError(const char* message)
{
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s: %s\n", message, SDL_GetError());
    return false;
}

bool Renderer::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        return logSdlError("Could not initialize SDL");
    }
    
    if (!SDL_CreateWindowAndRenderer("Renderer Window", wWidth * wScale, wHeight * wScale, 0, &window, &renderer))
    {
        return logSdlError("Could not create window and renderer");
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, wWidth, wHeight);
    if (!texture)
    {
        return logSdlError("Could not create texture");
    }

    if (wScale > 1 && !SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST))
    {
        return logSdlError("Could not set texture scale mode");
    }

    canvas = std::make_unique<Canvas>(wWidth, wHeight);

    return true;
}

void Renderer::render()
{
    if (!initialized) return;

    bool running = true;

    Raytracer raytracer(scene, wWidth, wHeight, 1, 1, 1);
    raytracer.raytraceImage(*canvas);

    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        SDL_UpdateTexture(texture, nullptr, canvas->getPixels(), wWidth * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }
}

void Renderer::cleanup()
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    canvas.reset();
    
    SDL_Quit();
}
