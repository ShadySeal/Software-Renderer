#include <SDL3/SDL.h>

namespace software_renderer
{
    struct ColorRGBA
    {
        const uint32_t color;

        ColorRGBA(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t alpha = 255)
            : color(SDL_MapRGBA(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32), nullptr, red, green, blue, alpha)) {}
    };
}
