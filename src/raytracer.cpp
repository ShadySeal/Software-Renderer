#include "raytracer.h"
#include "scene.h"
#include <iostream>

using namespace software_renderer;

Raytracer::Raytracer(Scene& scene, int cW, int cH, int vW, int vH, float d)
    : scene(scene), cW(cW), cH(cH), vW(vW), vH(vH), d(d) {}

Raytracer::~Raytracer() {}

void Raytracer::raytraceImage(Canvas& canvas) const
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

Vector3 Raytracer::canvasToViewport(float x, float y) const
{
    return Vector3(x * vW / cW, y * vH / cH, d);
}

uint32_t Raytracer::traceRay(const Vector3 o, const Vector3 d, const float tMin, const float tMax) const
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

    Vector3 p = o + d * closestT; // Compute intersection
    Vector3 n = p - closestSphere->center; // Compute sphere normal at intersection
    n = n / math::length(n);

    float i = computeLighting(p, n);

    Uint8 red = closestSphere->color.r * i;
    Uint8 green = closestSphere->color.g * i;
    Uint8 blue = closestSphere->color.b * i;
    Uint8 alpha = closestSphere->color.a;

    return SDL_MapRGBA(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32), nullptr, red, green, blue, alpha);
}

std::pair<float, float> Raytracer::intersectRaySphere(Vector3 o, Vector3 d, Sphere sphere) const
{
    float r = sphere.radius;
    Vector3 cO = o - sphere.center;

    float a = math::dot(d, d);
    float b = 2 * math::dot(cO, d);
    float c = math::dot(cO, cO) -r * r;

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
        return {inf, inf};
    }

    float t1 = (-b + sqrt(discriminant)) / (2 * a);
    float t2 = (-b - sqrt(discriminant)) / (2 * a);

    return {t1, t2};
}

float Raytracer::computeLighting(const Vector3 p, const Vector3 n) const
{
    float i = 0.0f;

    for (auto& light : scene.lights)
    {
        if (light.type == Light::AMBIENT)
        {
            i += light.getIntensity();
        }
        else
        {
            Vector3 l;

            if (light.type == Light::POINT)
            {
                l = light.position - p;
            }
            else
            {
                l = light.direction;
            }

            float nDotL = math::dot(n, l);

            if (nDotL > 0)
            {
                i += light.getIntensity() * nDotL/(math::length(n) * math::length(l));
            }
        }
    }

    return i;
}
