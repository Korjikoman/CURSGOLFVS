#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Entity.h"

#include "ECS.h"
#include "Components.h"

Entity* ball;
SDL_Renderer* Game::renderer = nullptr;

Map* map;

Manager manager;
auto& newPlayer(manager.addEntity());


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
    //ball = new Entity(Vector2f(0, 0), TextureManager::LoadTexture("assets/player.png"));
    map = new Map();
    newPlayer.addComponent<PositionComponent>();
    newPlayer.getComponent<PositionComponent>().setPos(500,500);
}

void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;

    default:
        break;
    }
}

void Game::update()
{
    //ball->Update();

    manager.update();
    std::cout << newPlayer.getComponent<PositionComponent>().x() << " , " << newPlayer.getComponent<PositionComponent>().y() << std::endl;

}

void Game::render()
{
    SDL_RenderClear(renderer);
 
    map->DrawMap();

    //ball->Renderer();

    SDL_RenderPresent(renderer);

}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "You've exited the game" << std::endl;
}
