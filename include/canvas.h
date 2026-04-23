#pragma once

#include <SDL3/SDL.h>

namespace software_renderer
{
    class Canvas
    {
    private:
        uint32_t* pixels;
        const int width;
        const int height;

    public:
        Canvas(int width, int height);
        ~Canvas();

        void setPixel(int x, int y, uint32_t color);
        uint32_t* getPixels() const;
    };
}
