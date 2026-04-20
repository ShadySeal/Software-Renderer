#include "renderer.h"

Renderer::Renderer(Scene& scene, int cW, int cH, int wS)
    : scene(scene), cW(cW), cH(cH), wS(wS), vW(1), vH(1), d(1) {}

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

Vector3 Renderer::canvasToViewport(float x, float y)
{
    return Vector3(x * vW / cW, y * vH / cH, d);
}

uint32_t Renderer::traceRay(Vector3 o, Vector3 d, float tMin, float tMax)
{
    float closestT = inf;
    Sphere* closestSphere = nullptr;

    for (auto& sphere : scene.spheres)
    {
        auto [t1, t2] = intersectRaySphere(o, d, sphere);

        if (t1 >= tMin && t1 <= tMax && t1 < closestT)
        {
            closestT = t1;
            closestSphere = &sphere;
        }

        if (t2 >= tMin && t2 <= tMax && t2 < closestT)
        {
            closestT = t2;
            closestSphere = &sphere;
        }
    }

    if (closestSphere == nullptr)
    {
        return getColor(0, 0, 0, 255);
    }

    return closestSphere->color;
}

std::pair<float, float> Renderer::intersectRaySphere(Vector3 o, Vector3 d, Sphere sphere)
{
    float r = sphere.radius;
    Vector3 cO = o - sphere.center;

    float a = dot(d, d);
    float b = 2 * dot(cO, d);
    float c = dot(cO, cO) -r * r;

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
        return {inf, inf};
    }

    float t1 = (-b + sqrt(discriminant)) / (2 * a);
    float t2 = (-b - sqrt(discriminant)) / (2 * a);
    return {t1, t2};
}

void Renderer::render()
{
    if (!init()) return;

    bool done = false;    

    std::cout << "Rendering...\n";

    Vector3 o(0, 0, 0);
    for (int x = -cW / 2; x <= cW / 2; x++)
    {
        for (int y = -cH / 2; y <= cH / 2; y++)
        {
            Vector3 d = canvasToViewport(x, y);
            uint32_t color = traceRay(o, d, 1, inf);
            canvas->setPixel(x, y, color);
        }
    }

    std::cout << "Rendering complete.\n";

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
