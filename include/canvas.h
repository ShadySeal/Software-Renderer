#pragma once

#include <SDL3/SDL.h>

namespace software_renderer
{
    class Canvas
    {
    private:
        uint32_t* _pixels;
        const int _width;
        const int _height;

    public:
        Canvas(int width, int height);
        ~Canvas();

        void setPixel(int x, int y, uint32_t color) const;
        uint32_t* getPixels() const;
    };
}
