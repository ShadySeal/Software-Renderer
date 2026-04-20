#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include <string>
#include "canvas.h"
#include "raytracer.h"

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

    bool init();

    void cleanup();

public:
    Renderer(Scene& scene, int cW, int cH, int wS = 1);
    ~Renderer();

    void render();

    uint32_t getColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) const;
};
