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


std::vector<ColliderComponent*> Game::colliders;


auto& newPlayer(manager.addEntity());
auto& hole(manager.addEntity());
auto& wall(manager.addEntity());


auto& tile0(manager.addEntity());
auto& tile1(manager.addEntity());
auto& tile2(manager.addEntity());

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
    newPlayer.addComponent<ColliderComponent>("ball");

    wall.addComponent<TransformComponent>(400.0f, 200.0f, 200, 200, 1);
    wall.addComponent<SpriteComponent>("assets/wall.png");
    wall.addComponent<ColliderComponent>("wall");

    hole.addComponent<TransformComponent>(800.0f, 200.0f, 40, 40, 1);
    hole.addComponent<SpriteComponent>("assets/hole.png");
    hole.addComponent<ColliderComponent>("wall");
  

    tile0.addComponent<TileComponent>(200, 200, 32, 32, 0);
    tile1.addComponent<TileComponent>(250, 250, 32, 32, 1);
    tile1.addComponent<ColliderComponent>("wall");
    tile2.addComponent<TileComponent>(150, 150, 32, 32, 2);
    tile2.addComponent<ColliderComponent>("water");

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

    //
    /*recA.x + recA.w >= recB.x &&
        recB.x + recB.w >= recA.x &&
        recA.y + recA.h >= recB.y &&
        recB.y + recB.h >= recA.y*/
        //


    for (auto cc : colliders)
    {
        if (cc->tag != "ball")
        if (Collision::AABB(newPlayer.getComponent<ColliderComponent>(), *cc)) {
            // ѕолучаем коллайдеры
            SDL_Rect* ball = &newPlayer.getComponent<ColliderComponent>().collider;
            SDL_Rect* entity = &cc->collider;
            // »з них получаем координаты
            int ballDeltaX = ball->x + ball->w;
            int ballDeltaY = ball->y + ball->h;
            

            int entityDeltaX = entity->x + entity->w;
            int entityDeltaY = entity->y + entity->h;
            // удар€емс€ сверху
            if ((ballDeltaY >= entity->y && ball->y < entity->y) || ((entityDeltaY >= ball->y && entity->y < ball->y)))
            {
                for (int i = 0; i <= 10; i++)
                std::cout << "UP OR DOWN\n";
                newPlayer.getComponent<TransformComponent>().velocity.y *= (-1); // мен€ем вектор y
                break;
            }
            // если удар€емс€ слева или справа
            if ((ballDeltaX >= entity->x && ball->x < entity->x) || ((entityDeltaX >= ball->x && entity->x < ball->x)))
            {
                for (int i = 0; i<= 10; i++)
                std::cout << "LEFT OR RIGHT\n";

                newPlayer.getComponent<TransformComponent>().velocity.x *= (-1);
                break;

            }
            
        }
        
    }

   /* if (Collision::AABB(newPlayer.getComponent<ColliderComponent>().collider,
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
    }*/
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
