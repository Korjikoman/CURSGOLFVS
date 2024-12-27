#include "Game.h"
#include "TextureManager.h"
#include "Map.h"

#include "ECS/ECS.h"
#include "ECS/Components.h"


Entity* ball;
SDL_Renderer* Game::renderer = nullptr;

Map* map;

Manager manager;
SDL_Event Game::event;

auto& newPlayer(manager.addEntity());
auto& newHole(manager.addEntity());


Game::Game()
{
}
Game::~Game()
{
}

void Game::init(const char *title, int x, int y, int width, int height, bool fullscreen)
{
    int flags = 0;
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "Subsytems Initiallized" << std::endl;
        window = SDL_CreateWindow(title, x, y, width, height, flags);
        if (window)
        {
            std::cout << "Window created!" << std::endl;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout
                << "Renderer created!" << std::endl;
        }
        isRunning = true;
    }
    else
    {
        isRunning = false;
    }
    map = new Map();

    // ecs implementation

    newPlayer.addComponent<TransformComponent>();
    newPlayer.addComponent<SpriteComponent>("assets/ball.png");
    newPlayer.addComponent<BallMechanic>();

    newHole.addComponent<TransformComponent>(800.0f, 300.0f);
    newHole.addComponent<SpriteComponent>("assets/hole.png");
    auto& hole = newHole.getComponent<Hole>();
    hole.setPlayer(&newPlayer);
}
bool mouseDown = false;
bool mousePressed = false;

void Game::handleEvents()
{
    
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;
    case SDL_MOUSEBUTTONDOWN :
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            mouseDown = true;
            mousePressed = true;
        }
        break;
    case SDL_MOUSEBUTTONUP:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            mouseDown = false;
        }
        break;
    default:
        break;
    }
}

bool Game::getMousePressed() {
    return mousePressed;
}

bool Game::getMouseDown() {
    return mouseDown;
}

void Game::update()
{
    manager.refresh();
    manager.update();

}

void Game::render()
{
    SDL_RenderClear(renderer);
 
    map->DrawMap();
    manager.draw();

    SDL_RenderPresent(renderer);

}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "You've exited the game" << std::endl;
}
