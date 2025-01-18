#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <fstream>
#include <map>

class RecordManager
{
public:
    RecordManager();
    ~RecordManager();

    void init(const std::string& recordFilePath, const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

    void updateRecord(int level, int currentScore);
    void render(int startX, int startY);
    bool isRunning() { return running; }
    int getSelectedOption() { return selectedOption; }
    void handleEvents();
private:
    TTF_Font* font;
    std::string recordsFile;
    std::map<int, int> bestRecords;
    int selectedOption;
    bool running;
    SDL_Window* recordsWindow;
    SDL_Renderer* recordsRenderer;

    void loadRecord();
    void saveRecord();

};