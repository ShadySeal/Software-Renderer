#pragma once

#include "vector3.h"
#include <SDL3/SDL.h>

namespace software_renderer
{
    struct Sphere
    {
        Vector3 center;
        float radius;
        SDL_Color color;

        Sphere(Vector3 c, float r, SDL_Color col) : center(c), radius(r), color(col) {}
    };
}
