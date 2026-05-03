#pragma once

#include <algorithm>
#include "vector3.h"

namespace software_renderer
{
    class Light
    {
    private:
        float intensity_;

    public:
        enum Type
        {
            AMBIENT,
            POINT,
            DIRECTIONAL
        };

        Type type;
        Vector3 position;
        Vector3 direction;

        Light(Type type, float intensity, Vector3 position = Vector3(0, 0, 0), Vector3 direction = Vector3(0, 0, 0))
            : type(type), intensity_(intensity), position(position), direction(direction) {}

        void setIntensity(float value)
        {
            intensity_ = std::clamp(value, 0.0f, 1.0f);
        }

        float getIntensity() const
        {
            return intensity_;
        }
    };
}