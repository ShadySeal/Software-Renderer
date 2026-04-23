#include "canvas.h"

using namespace software_renderer;

Canvas::Canvas(int width, int height) : width(width), height(height)
{
    pixels = new uint32_t[width * height];
}

Canvas::~Canvas()
{
    delete[] pixels;
}

void Canvas::setPixel(int x, int y, uint32_t color)
{
    int sx = width / 2 + x;
    int sy = height / 2 - y;
    if (sx >= 0 && sx < width && sy >= 0 && sy < height)
    {
        pixels[sy * width + sx] = color;
    }
}

uint32_t* Canvas::getPixels() const
{
    return pixels;
}
