#include "Game.h"
#include "TextureManager.h"
#include "Map.h"

#include "ECS/ECS.h"
#include "ECS/Components.h"
#include "Collision.h"

Entity* ball;
SDL_Renderer* Game::renderer = nullptr;

Map* map;

Manager manager;
SDL_Event Game::event;

auto& newPlayer(manager.addEntity());
auto& hole(manager.addEntity());
auto& wall(manager.addEntity());

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
    newPlayer.addComponent<ColliderComponent>("Player");

    wall.addComponent<TransformComponent>(400.0f, 200.0f, 200, 200, 1);
    wall.addComponent<SpriteComponent>("assets/wall.png");
    wall.addComponent<ColliderComponent>("wall");

    hole.addComponent<TransformComponent>(800.0f, 200.0f, 40, 40, 1);
    hole.addComponent<SpriteComponent>("assets/hole.png");
    hole.addComponent<ColliderComponent>("wall");
    
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

    if (Collision::AABB(newPlayer.getComponent<ColliderComponent>().collider,
        wall.getComponent<ColliderComponent>().collider))
    {
        
        int colliderPlace = Collision::AABBplace(newPlayer.getComponent<ColliderComponent>().collider,
            wall.getComponent<ColliderComponent>().collider);
        switch (colliderPlace)
        {
        case 1:newPlayer.getComponent<TransformComponent>().velocity * -1;
        default:
            break;
        }
        
       
        std::cout << "Wall Hit!" << std::endl;
    }
    if (Collision::AABB(newPlayer.getComponent<ColliderComponent>().collider,
        hole.getComponent<ColliderComponent>().collider))
    {
        newPlayer.getComponent<TransformComponent>().position.x = 300.0f;
        newPlayer.getComponent<TransformComponent>().position.y = 300.0f;
        newPlayer.getComponent<TransformComponent>().velocity.x = 0;
        newPlayer.getComponent<TransformComponent>().velocity.y = 0;
    }
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
