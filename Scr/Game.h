#pragma once 
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>

class ColliderComponent;

class Game
{
public:
    Game();
    ~Game();
    void init(const char *title, int x, int y, int width, int height, bool fullscreen);

    void handleEvents();
    bool getMousePressed();
    bool getMouseDown();
    void update();
    void render();
    void clean();

    bool running() { return isRunning; }
    static SDL_Renderer* renderer;
    static SDL_Event event;
    static std::vector<ColliderComponent*> colliders;
    static bool win; // ƒобавл€ем переменную win как статическую
    void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y);
private:
    int counter = 0;
    bool isRunning;
    SDL_Window *window;
};