#include "Game.h"
#include "TextureManager.h"
#include "Map.h"

#include "ECS/ECS.h"
#include "ECS/Components.h"
#include "Collision.h"
#include "SDL_ttf.h"

Entity* ball;
SDL_Renderer* Game::renderer = nullptr;

Map* map;

Manager manager;
SDL_Event Game::event;
bool Game::win = false; // Инициализация переменной win

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
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
        isRunning = false;
    }
    map = new Map();

    // ecs implementation

    newPlayer.addComponent<TransformComponent>(100.0f, 200.0f, 32, 32, 1);
    newPlayer.addComponent<SpriteComponent>("assets/ball.png");
    newPlayer.addComponent<BallMechanic>();
    newPlayer.addComponent<ColliderComponent>("ball");

    wall.addComponent<TransformComponent>(400.0f, 200.0f, 200, 200, 1);
    wall.addComponent<SpriteComponent>("assets/wall.png");
    wall.addComponent<ColliderComponent>("wall");

    wall.addComponent<TransformComponent>(32.0f, 0.0f, 32, 896, 1);
    wall.addComponent<SpriteComponent>("assets/borderup.png");
    wall.addComponent<ColliderComponent>("wall");

    wall.addComponent<TransformComponent>(0.0f, 32.0f, 566, 32, 1);
    wall.addComponent<SpriteComponent>("assets/borderleft.png");
    wall.addComponent<ColliderComponent>("wall");

    wall.addComponent<TransformComponent>(928.0f, 32.0f, 566, 32, 1);
    wall.addComponent<SpriteComponent>("assets/borderright.png");
    wall.addComponent<ColliderComponent>("wall");

    wall.addComponent<TransformComponent>(32.0f, 598.0f, 32, 896, 1);
    wall.addComponent<SpriteComponent>("assets/borderdown.png");
    wall.addComponent<ColliderComponent>("wall");

    wall.addComponent<TransformComponent>(0.0f, 0.0f, 32, 32, 1);
    wall.addComponent<SpriteComponent>("assets/border.png");
    wall.addComponent<ColliderComponent>("wall");

    wall.addComponent<TransformComponent>(928.0f, 598.0f, 32, 32, 1);
    wall.addComponent<SpriteComponent>("assets/border.png");
    wall.addComponent<ColliderComponent>("wall");

    wall.addComponent<TransformComponent>(928.0f, 0.0f, 32, 32, 1);
    wall.addComponent<SpriteComponent>("assets/border.png");
    wall.addComponent<ColliderComponent>("wall");

    wall.addComponent<TransformComponent>(0.0f, 598.0f, 32, 32, 1);
    wall.addComponent<SpriteComponent>("assets/border.png");
    wall.addComponent<ColliderComponent>("wall");

    hole.addComponent<TransformComponent>(800.0f, 200.0f, 40, 40, 1);
    hole.addComponent<SpriteComponent>("assets/hole.png");
    hole.addComponent<ColliderComponent>("hole");
  

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
                // Получаем коллайдеры
                SDL_Rect* ball = &newPlayer.getComponent<ColliderComponent>().collider;
                SDL_Rect* entity = &cc->collider;
                if (ball->x < entity->x + entity->w &&
                    ball->x + ball->w > entity->x &&
                    ball->y < entity->y + entity->h &&
                    ball->y + ball->h > entity->y &&
                    cc->tag != "hole")
                {

                    // Определяем, с какой стороны произошло столкновение
                    int ballCenterX = ball->x + ball->w / 2;
                    int ballCenterY = ball->y + ball->h / 2;
                    int entityCenterX = entity->x + entity->w / 2;
                    int entityCenterY = entity->y + entity->h / 2;

                    int deltaX = ballCenterX - entityCenterX;
                    int deltaY = ballCenterY - entityCenterY;

                    int intersectX = (ball->w / 2 + entity->w / 2) - abs(deltaX);
                    int intersectY = (ball->h / 2 + entity->h / 2) - abs(deltaY);

                    // Если пересечение по X больше, чем по Y, то столкновение произошло сверху или снизу
                    if (intersectX > intersectY) {
                        if (deltaY > 0) {
                            std::cout << "COLLISION FROM TOP\n";
                            newPlayer.getComponent<TransformComponent>().velocity.y *= -1;
                            newPlayer.getComponent<TransformComponent>().acceleration.y *= -1;
                        }
                        else {
                            std::cout << "COLLISION FROM BOTTOM\n";
                            newPlayer.getComponent<TransformComponent>().velocity.y *= -1;
                            newPlayer.getComponent<TransformComponent>().acceleration.y *= -1;
                        }
                    }
                    else {
                        if (deltaX > 0) {
                            std::cout << "COLLISION FROM LEFT\n";
                            newPlayer.getComponent<TransformComponent>().velocity.x *= -1;
                            newPlayer.getComponent<TransformComponent>().acceleration.x *= -1;
                        }
                        else {
                            std::cout << "COLLISION FROM RIGHT\n";
                            newPlayer.getComponent<TransformComponent>().velocity.x *= -1;
                            newPlayer.getComponent<TransformComponent>().acceleration.x *= -1;
                        }
                    }

                }
                else if (cc->tag == "hole")
                {
                    newPlayer.getComponent<TransformComponent>().position.x = 300.0f;
                    newPlayer.getComponent<TransformComponent>().position.y = 300.0f;
                    newPlayer.getComponent<TransformComponent>().velocity.x = 0;
                    newPlayer.getComponent<TransformComponent>().velocity.y = 0;
                    newPlayer.getComponent<BallMechanic>().strokes = 0;
                }

            }
    }
}

void Game::render()
{
    SDL_RenderClear(renderer);
 
    map->DrawMap();
    manager.draw();
    TTF_Font* font = TTF_OpenFont("assets/font/font.ttf", 24); // Путь к вашему шрифту, размер шрифта
    if (!font) {
        std::cerr << "Не удалось загрузить шрифт: " << TTF_GetError() << std::endl;
    }
    std::string strokesText = "Strokes: " + std::to_string(newPlayer.getComponent<BallMechanic>().strokes);
    renderText(renderer, font, strokesText, 33, 0); // Текст в левом верхнем углу

    SDL_RenderPresent(renderer);

}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "You've exited the game" << std::endl;
}

void Game::renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y)
{
    SDL_Color textColor = { 0, 0, 0, 255 }; // Черный цвет текста

    // Создаём поверхность с текстом
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if (!surface) {
        std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
        return;
    }

    // Создаём текстуру из поверхности
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    // Устанавливаем размер текста
    SDL_Rect destRect = { x, y, surface->w, surface->h };

    // Освобождаем поверхность
    SDL_FreeSurface(surface);

    // Отображаем текстуру
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);

    // Удаляем текстуру после отрисовки
    SDL_DestroyTexture(texture);
}

