#pragma once

#include <SDL3/SDL.h>

namespace software_renderer
{
    struct ColorRGBA
    {
        const uint32_t color;

        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t alpha;

        ColorRGBA(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t alpha = 255)
            : color(SDL_MapRGBA(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32), nullptr, red, green, blue, alpha)),
            red(red), green(green), blue(blue), alpha(alpha) {}

        ColorRGBA operator*(float value)
        {
            return ColorRGBA(red * value, green * value, blue * value);
        }
    };
}
