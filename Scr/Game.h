#pragma once 
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS/ECS.h"
#include "ECS/Components.h"
#include "TextureManager.h"
#include "Map.h"

#include <sstream>

#include "Collision.h"
#include "SDL_mixer.h"



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
    static bool win; // ƒобавл€ем переменную win как статическую
    void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y);
    void loadLevel(const char* mapPath, int playerPositionX, int playerPositionY, int holePositionX, int holePositionY,
        int flagPositionX, int flagPositionY);
    void newLevelStart();

    enum groupLabels : std::size_t
    {
        groupMap,
        groupBall,
        groupColliders,
        groupHole,
        groupBorder,
        groupBooster,
        groupFlag
    };

    Manager& manager;

private:
    int counter = 0;
    bool isRunning;
    SDL_Window *window;
    std::vector<std::string> musicTracks;
    int currentTrackIndex = 0;
    bool isMusicPlaying = false;

    Entity* newPlayer;
    Entity* hole;
    Entity* hole1;
    Entity* flag;
    Entity* flag1;
    Entity* wall;
    Entity* box;


};