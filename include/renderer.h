#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include <string>

class Renderer
{
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;

    const int wWidth;
    const int wHeight;

    bool init();
    uint32_t getColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) const;
    void drawPoint(uint32_t* pixels, int px, int py, int size, uint32_t color);
    void cleanup();

public:
    Renderer(const int wWidth, const int wHeight);
    ~Renderer();

    void render();
};
