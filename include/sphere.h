#pragma once

#include "vector3.h"
#include <SDL3/SDL.h>

namespace software_renderer
{
    struct Sphere
    {
        Vector3 center;
        float radius;
        uint32_t color;

        Sphere(Vector3 c, float r, uint32_t col) : center(c), radius(r), color(col) {}
    };
}
