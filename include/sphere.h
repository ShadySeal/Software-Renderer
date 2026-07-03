#pragma once

#include "vector3.h"
#include "color_rgba.h"

namespace software_renderer
{
    struct Sphere
    {
        Vector3 center;
        float radius;
        ColorRGBA color;
        float specular;
        float reflective;

        Sphere(Vector3 center, float radius, ColorRGBA color, float specular, float reflective) 
            : center(center), radius(radius), color(color), specular(specular), reflective(reflective) {}
    };
}
