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
        Scene& _scene;

        const int _cW;
        const int _cH;

        const float _vW;
        const float _vH;
        const float _d;

        const float _inf = std::numeric_limits<float>::infinity();

        Vector3 canvasToViewport(const float x, const float y) const;
        uint32_t traceRay(const Vector3 o, const Vector3 d, const float tMin, const float tMax) const;
        std::pair<float, float> intersectRaySphere(const Vector3 o, const Vector3 d, const Sphere sphere) const;
        float computeLighting(const Vector3 p, const Vector3 n) const;

    public:
        Raytracer(Scene& scene, int cW, int cH, float vW, float vH, float d);
        ~Raytracer();

        void raytraceImage(Canvas& canvas) const;
    };
}
