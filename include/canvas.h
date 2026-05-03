#pragma once

#include <SDL3/SDL.h>

namespace software_renderer
{
    class Canvas
    {
    private:
        uint32_t* pixels_;
        const int width_;
        const int height_;

    public:
        Canvas(int width, int height);
        ~Canvas();

        void setPixel(int x, int y, uint32_t color) const;
        uint32_t* getPixels() const;
    };
}
