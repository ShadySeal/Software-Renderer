#include "scene.h"
#include "renderer.h"

Scene::Scene()
{
    Renderer renderer(*this, 480*2, 480*2, 1);

    Sphere redSphere(Vector3(0, -1, 3), 1, renderer.getColor(255, 0, 0, 255));
    Sphere blueSphere(Vector3(2, 0, 4), 1, renderer.getColor(0, 0, 255, 255));
    Sphere greenSphere(Vector3(-2, 0, 4), 1, renderer.getColor(0, 255, 0, 255));

    spheres = {redSphere, blueSphere, greenSphere};

    renderer.render();
}
Scene::~Scene() {}