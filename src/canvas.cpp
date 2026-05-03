#include "canvas.h"

using namespace software_renderer;

Canvas::Canvas(int width, int height) : width_(width), height_(height)
{
    pixels_ = new uint32_t[width * height];
}

Canvas::~Canvas()
{
    delete[] pixels_;
}

void Canvas::setPixel(int x, int y, uint32_t color) const
{
    const int sx = width_ / 2 + x;
    const int sy = height_ / 2 - y;
    if (sx >= 0 && sx < width_ && sy >= 0 && sy < height_)
    {
        pixels_[sy * width_ + sx] = color;
    }
}

uint32_t* Canvas::getPixels() const
{
    return pixels_;
}
