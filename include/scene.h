#pragma once

#include <vector>
#include "sphere.h"
#include "color_rgba.h"

namespace software_renderer
{
    struct Scene
    {
        Scene();
        ~Scene();

        std::vector<Sphere> spheres;
    };
}