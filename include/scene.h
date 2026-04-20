#pragma once

#include <vector>
#include "sphere.h"

struct Scene
{
    Scene();
    ~Scene();

    std::vector<Sphere> spheres;
};