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

        Sphere(Vector3 c, float r, ColorRGBA col, float s) : center(c), radius(r), color(col), specular(s) {}
    };
}
