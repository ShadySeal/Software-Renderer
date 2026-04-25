#pragma once

#include <vector>
#include "sphere.h"
#include "color_rgba.h"
#include "light.h"
#include <SDL3/SDL.h>

namespace software_renderer
{
    struct Scene
    {
        Scene();
        ~Scene();

        ColorRGBA backgroundColor;

        std::vector<Sphere> spheres;
        std::vector<Light> lights;
    };
}