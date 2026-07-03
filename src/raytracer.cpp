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
    Vector3 o(0, 0, 0);
    for (int x = -_cW / 2; x <= _cW / 2; x++)
    {
        for (int y = -_cH / 2; y <= _cH / 2; y++)
        {
            Vector3 d = canvasToViewport(x, y);
            ColorRGBA color = traceRay(o, d, 1, _inf, _recursionDepth);
            canvas.setPixel(x, y, color.packed);
        }
    }
}

Vector3 Raytracer::canvasToViewport(const float x, const float y) const
{
    return Vector3(x * _vW / _cW, y * _vH / _cH, _d);
}

ColorRGBA Raytracer::traceRay(const Vector3 o, const Vector3 d, const float tMin, const float tMax, const int recursionDepth) const
{
    auto [closestT, closestSphere] = closestIntersection(o, d, tMin, tMax);

    if (closestSphere == nullptr)
    {
        return _scene.backgroundColor;
    }

    const Vector3 p = o + d * closestT; // Compute intersection
    Vector3 n = p - closestSphere->center; // Compute sphere normal at intersection
    n.normalize();

    ColorRGBA localColor = closestSphere->color * computeLighting(p, n, d * -1, closestSphere->specular);

    // If we hit the recursion limit or the object is not reflective, we're done
    float reflectiveness = closestSphere->reflective;
    if (recursionDepth <= 0 || reflectiveness <= 0)
    {
        return localColor;
    }

    // Compute the reflected color
    const Vector3 r = reflectRay(d * -1, n);
    const ColorRGBA reflectedColor = traceRay(p, r, 0.1f, _inf, recursionDepth - 1);

    return localColor * (1 - reflectiveness) + reflectedColor * reflectiveness;
}

std::pair<float, const Sphere*> Raytracer::closestIntersection(const Vector3 o, const Vector3 d, const float tMin, const float tMax) const
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

    return {closestT, closestSphere};
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

float Raytracer::computeLighting(const Vector3 p, const Vector3 n, const Vector3 v, const float s) const
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
            float tMax;

            if (light.type == Light::POINT)
            {
                l = light.position - p;
                tMax = 1;
            }
            else
            {
                l = light.direction;
                tMax = _inf;
            }

            // Shadow check
            auto [shadowT, shadowSphere] = closestIntersection(p, l, 0.001, tMax);
            if (shadowSphere != nullptr)
            {
                continue;
            }

            // Diffuse
            const float nDotL = Vector3::dot(n, l);
            if (nDotL > 0)
            {
                i += light.intensity * nDotL / (n.magnitude() * l.magnitude());
            }

            // Specular
            if (s != -1)
            {
                const Vector3 r = reflectRay(l, n);
                const float rDotV = Vector3::dot(r, v);
                if (rDotV > 0)
                {
                    i += light.intensity * pow(rDotV / (r.magnitude() * v.magnitude()), s);
                }
            }
        }
    }

    return i;
}

Vector3 Raytracer::reflectRay(const Vector3 r, const Vector3 n) const
{
    return n * 2 * Vector3::dot(n, r) - r;
}
