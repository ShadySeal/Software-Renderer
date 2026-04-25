#include "scene.h"
#include "renderer.h"
#include "color_rgba.h"

using namespace software_renderer;

Scene::Scene()
{
    backgroundColor = ColorRGBA(0, 128, 255, 255);

    Light ambient(Light::AMBIENT, 0.2);
    Light point(Light::POINT, 0.6, Vector3(2, 1, 0));
    Light directional(Light::DIRECTIONAL, 0.2, Vector3(0, 0, 0), Vector3(1, 4, 4));

    lights = {ambient, point, directional};

    Sphere redSphere(Vector3(0, -1, 3), 1, ColorRGBA(255, 0, 0, 255));
    Sphere blueSphere(Vector3(2, 0, 4), 1, ColorRGBA(0, 0, 255, 255));
    Sphere greenSphere(Vector3(-2, 0, 4), 1, ColorRGBA(0, 255, 0, 255));
    Sphere yellowSphere(Vector3(0, -5001, 0), 5000, ColorRGBA(255, 255, 0, 255));

    spheres = {redSphere, blueSphere, greenSphere, yellowSphere};

    Renderer renderer(*this, 800, 800, 1);
    renderer.render();
}

Scene::~Scene() {}
