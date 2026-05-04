#include "raytracer.h"
#include "scene.h"
#include "color_rgba.h"
#include <iostream>

using namespace software_renderer;

Raytracer::Raytracer(Scene& scene, int cW, int cH, float vW, float vH, float d)
    : _scene(scene), _cW(cW), _cH(cH), _vW(vW), _vH(vH), _d(d) {}

Raytracer::~Raytracer() {}

void Raytracer::raytraceImage(Canvas& canvas) const
{
    Vector3 o(0, 0, -3);
    for (int x = -_cW / 2; x <= _cW / 2; x++)
    {
        for (int y = -_cH / 2; y <= _cH / 2; y++)
        {
            Vector3 d = canvasToViewport(x, y);
            uint32_t color = traceRay(o, d, 1, _inf);
            canvas.setPixel(x, y, color);
        }
    }
}

Vector3 Raytracer::canvasToViewport(const float x, const float y) const
{
    return Vector3(x * _vW / _cW, y * _vH / _cH, _d);
}

uint32_t Raytracer::traceRay(const Vector3 o, const Vector3 d, const float tMin, const float tMax) const
{
    float closestT = _inf;
    Sphere* closestSphere = nullptr;

    for (auto& sphere : _scene.spheres)
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
        return _scene.backgroundColor.packed;
    }

    const Vector3 p = o + d * closestT; // Compute intersection
    Vector3 n = p - closestSphere->center; // Compute sphere normal at intersection
    n.normalize();

    ColorRGBA litColor = closestSphere->color * computeLighting(p, n);
    return litColor.packed;
}

std::pair<float, float> Raytracer::intersectRaySphere(const Vector3 o, const Vector3 d, const Sphere sphere) const
{
    const float r = sphere.radius;
    const Vector3 cO = o - sphere.center;

    const float a = Vector3::dot(d, d);
    const float b = 2 * Vector3::dot(cO, d);
    const float c = Vector3::dot(cO, cO) -r * r;

    const float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
        return {_inf, _inf};
    }

    const float t1 = (-b + sqrt(discriminant)) / (2 * a);
    const float t2 = (-b - sqrt(discriminant)) / (2 * a);

    return {t1, t2};
}

float Raytracer::computeLighting(const Vector3 p, const Vector3 n) const
{
    float i = 0.0f;

    for (auto& light : _scene.lights)
    {
        if (light.type == Light::AMBIENT)
        {
            i += light.intensity;
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

            const float nDotL = Vector3::dot(n, l);

            if (nDotL > 0)
            {
                i += light.intensity * nDotL / (n.magnitude() * l.magnitude());
            }
        }
    }

    return i;
}
