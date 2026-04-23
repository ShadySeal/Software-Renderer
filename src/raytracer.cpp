#include "raytracer.h"
#include "scene.h"
#include <iostream>

using namespace software_renderer;

Raytracer::Raytracer(Scene& scene, int cW, int cH, int vW, int vH, int d)
    : scene(scene), cW(cW), cH(cH), vW(vW), vH(vH), d(d) {}

Raytracer::~Raytracer() {}

void Raytracer::raytraceImage(Canvas& canvas)
{
    Vector3 o(0, 0, 0);
    for (int x = -cW / 2; x <= cW / 2; x++)
    {
        for (int y = -cH / 2; y <= cH / 2; y++)
        {
            Vector3 d = canvasToViewport(x, y);
            uint32_t color = traceRay(o, d, 1, inf);
            canvas.setPixel(x, y, color);
        }
    }
}

Vector3 Raytracer::canvasToViewport(float x, float y)
{
    return Vector3(x * vW / cW, y * vH / cH, d);
}

uint32_t Raytracer::traceRay(Vector3 o, Vector3 d, float tMin, float tMax)
{
    float closestT = inf;
    Sphere* closestSphere = nullptr;

    for (auto& sphere : scene.spheres)
    {
        auto [t1, t2] = intersectRaySphere(o, d, sphere);

        if (t1 >= tMin && t1 <= tMax && t1 < closestT)
        {
            closestT = t1;
            closestSphere = &sphere;
        }

        if (t2 >= tMin && t2 <= tMax && t2 < closestT)
        {
            closestT = t2;
            closestSphere = &sphere;
        }
    }

    if (closestSphere == nullptr)
    {
        return 0xff00000; // Black
    }

    return closestSphere->color;
}

std::pair<float, float> Raytracer::intersectRaySphere(Vector3 o, Vector3 d, Sphere sphere)
{
    float r = sphere.radius;
    Vector3 cO = o - sphere.center;

    float a = dot(d, d);
    float b = 2 * dot(cO, d);
    float c = dot(cO, cO) -r * r;

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
        return {inf, inf};
    }

    float t1 = (-b + sqrt(discriminant)) / (2 * a);
    float t2 = (-b - sqrt(discriminant)) / (2 * a);

    return {t1, t2};
}
