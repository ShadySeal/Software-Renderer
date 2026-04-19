#include "renderer.h"

Renderer::Renderer(const int wWidth, const int wHeight)
    : wWidth(wWidth), wHeight(wHeight) {}

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
    
    window = SDL_CreateWindow("Renderer Window", wWidth, wHeight, 0);
    if (!window)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer: %s\n", SDL_GetError());
        return false;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, wWidth, wHeight);
    if (!texture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create texture: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

uint32_t Renderer::getColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) const
{
    return SDL_MapRGBA(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32), nullptr, red, green, blue, alpha);
}

void Renderer::drawPoint(uint32_t* pixels, int px, int py, int size, uint32_t color)
{
    for (int y = -size; y <= size; y++)
    {
        for (int x = -size; x <= size; x++)
        {
            int sx = px + x;
            int sy = py + y;

            // bounds check
            if (sx >= 0 && sx < wWidth && sy >= 0 && sy < wHeight)
            {
                pixels[sy * wWidth + sx] = color;
            }
        }
    }
}

void Renderer::render()
{
    if (!init()) return;

    bool done = false;    

    uint32_t pixels[wWidth * wHeight] = {};

    drawPoint(pixels, wWidth / 2, wHeight / 2, 3, getColor(0, 255, 0, 255));

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

        SDL_UpdateTexture(texture, nullptr, pixels, wWidth * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    cleanup();
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
    
    SDL_Quit();
}
