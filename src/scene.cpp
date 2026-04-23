#include "scene.h"
#include "renderer.h"

using namespace software_renderer;

Scene::Scene()
{
    Sphere redSphere(Vector3(0, -1, 3), 1, ColorRGBA(255, 0, 0, 255).color);
    Sphere blueSphere(Vector3(2, 0, 4), 1, ColorRGBA(0, 0, 255, 255).color);
    Sphere greenSphere(Vector3(-2, 0, 4), 1, ColorRGBA(0, 255, 0, 255).color);

    spheres = {redSphere, blueSphere, greenSphere};

    Renderer renderer(*this, 800, 800, 1);
    renderer.render();
}
Scene::~Scene() {}
