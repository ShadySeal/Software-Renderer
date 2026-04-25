#pragma once

#include <iostream>
#include <limits>
#include <utility>
#include <cmath>
#include <SDL3/SDL.h>
#include "vector3.h"
#include "sphere.h"
#include "canvas.h"

namespace software_renderer
{
    struct Scene;

    class Raytracer
    {
    private:
        Scene& scene;

        const int cW;
        const int cH;

        const int vW;
        const int vH;
        const float d;

        const float inf = std::numeric_limits<float>::infinity();

        Vector3 canvasToViewport(float x, float y) const;
        uint32_t traceRay(const Vector3 o, const Vector3 d, const float tMin, const float tMax) const;
        std::pair<float, float> intersectRaySphere(Vector3 o, Vector3 d, Sphere sphere) const;
        float computeLighting(const Vector3 p, const Vector3 n) const;

    public:
        Raytracer(Scene& scene, int cW, int cH, int vW, int vH, float d);
        ~Raytracer();

        void raytraceImage(Canvas& canvas) const;
    };
}
