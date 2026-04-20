#include "renderer.h"

Renderer::Renderer(Scene& scene, int cW, int cH, int wS)
    : scene(scene), cW(cW), cH(cH), wS(wS) {}

Renderer::~Renderer()
{
    cleanup();
}

bool Renderer::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initialize SDL: %s\n", SDL_GetError());
        return false;
    }
    
    if (!SDL_CreateWindowAndRenderer("Renderer Window", cW * wS, cH * wS, 0, &window, &renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window and renderer: %s\n", SDL_GetError());
        return false;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, cW, cH);
    if (!texture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create texture: %s\n", SDL_GetError());
        return false;
    }

    if (wS > 1)
    {
        if (!SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST))
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not set texture scale mode: %s\n", SDL_GetError());
            return false;
        }
    }

    canvas = new Canvas(cW, cH);

    return true;
}

uint32_t Renderer::getColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) const
{
    return SDL_MapRGBA(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32), nullptr, red, green, blue, alpha);
}

void Renderer::render()
{
    if (!init()) return;

    bool done = false;

    Raytracer raytracer(scene, cW, cH, 1, 1, 1);

    raytracer.drawPixels(*canvas);

    while (!done)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                done = true;
            }
        }

        SDL_UpdateTexture(texture, nullptr, canvas->getPixels(), cW * sizeof(uint32_t));
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

    delete canvas;
    canvas = nullptr;
    
    SDL_Quit();
}
