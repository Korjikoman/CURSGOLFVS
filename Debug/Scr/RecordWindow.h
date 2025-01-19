#ifndef RECORDWINDOW_H
#define RECORDWINDOW_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>

class RecordWindow {
public:
    RecordWindow();
    ~RecordWindow();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleEvents();
    void render();
    bool isRunning() const;

private:
    SDL_Window* recordWindow;
    SDL_Renderer* recordRenderer;
    TTF_Font* font;
    bool running;

    std::vector<std::string> loadRecords(const std::string& filepath);
};

#endif // RECORDWINDOW_H
