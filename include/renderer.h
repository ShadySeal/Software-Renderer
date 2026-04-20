#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include <string>
#include "canvas.h"
#include "vector3.h"
#include <limits>
#include "scene.h"
#include "sphere.h"
#include <utility>
#include <cmath>

class Renderer
{
private:
    Scene& scene;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
    Canvas* canvas = nullptr;

    const int cW;
    const int cH;
    const int wS;

    const int vW;
    const int vH;
    const int d;

    const float inf = std::numeric_limits<float>::infinity();

    bool init();

    Vector3 canvasToViewport(float x, float y);
    uint32_t traceRay(Vector3 o, Vector3 d, float tMin, float tMax);
    std::pair<float, float> intersectRaySphere(Vector3 o, Vector3 d, Sphere sphere);

    void cleanup();

public:
    Renderer(Scene& scene, int cW, int cH, int wS = 1);
    ~Renderer();

    void render();

    uint32_t getColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) const;

    float dot(Vector3 const& a, Vector3 const& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
};
