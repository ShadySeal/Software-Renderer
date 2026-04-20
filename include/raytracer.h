#pragma once

#include <iostream>
#include <limits>
#include <utility>
#include <cmath>
#include <SDL3/SDL.h>
#include "vector3.h"
#include "sphere.h"
#include "scene.h"
#include "renderer.h"
#include "canvas.h"


class Raytracer
{
private:
    Scene& scene;

    const int cW;
    const int cH;

    const int vW;
    const int vH;
    const int d;

    const float inf = std::numeric_limits<float>::infinity();

    Vector3 canvasToViewport(float x, float y);
    uint32_t traceRay(Vector3 o, Vector3 d, float tMin, float tMax);
    std::pair<float, float> intersectRaySphere(Vector3 o, Vector3 d, Sphere sphere);

public:
    Raytracer(Scene& scene, int cW, int cH, int vW, int vH, int d);
    ~Raytracer();

    void drawPixels(Canvas& canvas);
};