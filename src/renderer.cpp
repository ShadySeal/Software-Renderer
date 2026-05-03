#include "renderer.h"
#include "raytracer.h"
#include "scene.h"

using namespace software_renderer;

Renderer::Renderer(Scene& scene, int wWidth, int wHeight, int wScale)
    : scene_(scene), wWidth_(wWidth), wHeight_(wHeight), wScale_(wScale)
{
    initialized_ = init();
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
    
    if (!SDL_CreateWindowAndRenderer("Renderer Window", wWidth_ * wScale_, wHeight_ * wScale_, 0, &window_, &renderer_))
    {
        return logSdlError("Could not create window and renderer");
    }

    texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, wWidth_, wHeight_);
    if (!texture_)
    {
        return logSdlError("Could not create texture");
    }

    if (wScale_ > 1 && !SDL_SetTextureScaleMode(texture_, SDL_SCALEMODE_NEAREST))
    {
        return logSdlError("Could not set texture scale mode");
    }

    canvas_ = std::make_unique<Canvas>(wWidth_, wHeight_);

    return true;
}

void Renderer::render()
{
    if (!initialized_) return;

    bool running = true;

    Raytracer raytracer(scene_, wWidth_, wHeight_, 1, 1, 1);
    raytracer.raytraceImage(*canvas_);

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

        SDL_UpdateTexture(texture_, nullptr, canvas_->getPixels(), wWidth_ * sizeof(uint32_t));
        SDL_RenderClear(renderer_);
        SDL_RenderTexture(renderer_, texture_, nullptr, nullptr);
        SDL_RenderPresent(renderer_);
    }
}

void Renderer::cleanup()
{
    if (texture_)
    {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }

    if (renderer_)
    {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }

    if (window_)
    {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }

    canvas_.reset();
    
    SDL_Quit();
}
