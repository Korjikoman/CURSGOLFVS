#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>

class Menu {
public:
    Menu();
    ~Menu();
    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleEvents();
    void render();
    bool isRunning() { return running; }
    int getSelectedOption() { return selectedOption; }

private:
    bool running;
    int selectedOption;
    SDL_Window* menuWindow;
    SDL_Renderer* menuRenderer;
    TTF_Font* font;
    std::vector<std::string> menuOptions;
};
