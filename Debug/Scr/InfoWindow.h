#ifndef INFOWINDOW_H
#define INFOWINDOW_H

#include <SDL.h>
#include <SDL_ttf.h>

class InfoWindow {
public:
    InfoWindow();
    ~InfoWindow();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleEvents();
    void render();
    bool isRunning() const;

private:
    bool running;
    SDL_Window* infoWindow;
    SDL_Renderer* infoRenderer;
    TTF_Font* font;
};

#endif
