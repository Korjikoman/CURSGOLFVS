#include "Game.h"
#include "TextureManager.h"
#include "Map.h"

#include <sstream>
#include "ECS/ECS.h"
#include "ECS/Components.h"
#include "Collision.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

Entity* ball;
SDL_Renderer* Game::renderer = nullptr;

Map* map;

Manager manager;
SDL_Event Game::event;
bool Game::win = false; // Инициализация переменной win
int currentLevel = 1;
std::vector<ColliderComponent*> Game::colliders;
Mix_Chunk* holeSound = nullptr;
Mix_Chunk* budaSound = nullptr;
Uint32 startTime; // Время начала таймера
float elapsedTime; // Прошедшее время в секундах


const char* mapfile = "assets/newtileset.png";


auto& newPlayer(manager.addEntity());
auto& hole(manager.addEntity());
auto& wall(manager.addEntity());
auto& box(manager.addEntity());

enum groupLabels : std::size_t
{
    groupMap,
    groupBall,
    groupColliders,
    groupHole,
    groupBorder,
    groupBooster
};

auto& tile0(manager.addEntity());
auto& tile1(manager.addEntity());
auto& tile2(manager.addEntity());
auto& tilebox1(manager.addEntity());
auto& tilebox2(manager.addEntity());
auto& tilebox3(manager.addEntity());

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
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        }
        holeSound = Mix_LoadWAV("assets/sfx/hole.mp3");
        if (!holeSound) {
            std::cerr << "Failed to load hole sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
        }
        budaSound = Mix_LoadWAV("assets/sfx/bude.mp3");
        if (!budaSound) {
            std::cerr << "Failed to load buda sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
        }
        if (budaSound) {
            Mix_PlayChannel(-1, budaSound, 10);
        }
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

    startTime = SDL_GetTicks(); // Записываем время начала в миллисекундах
    elapsedTime = 0; // Инициализируем прошедшее время

    // ecs implementation

    Map::LoadMap("assets/map.map", 30, 19);

    newPlayer.addComponent<TransformComponent>(100.0f, 100.0f, 32, 32, 1);
    newPlayer.addComponent<SpriteComponent>("assets/ball.png");
    newPlayer.addComponent<BallMechanic>();
    newPlayer.addComponent<ColliderComponent>("ball");
    newPlayer.addGroup(groupBall);

    // верхняя стенка
    wall.addComponent<TransformComponent>(32.0f, 0.0f, 32, 896, 1);
    wall.addComponent<SpriteComponent>("assets/borderup.png");
    wall.addComponent<ColliderComponent>("wall");
    wall.addGroup(groupBorder);

    // стенка слева 
    auto& wall1(manager.addEntity());
    wall1.addComponent<TransformComponent>(0.0f, 32.0f, 566, 32, 1);
    wall1.addComponent<SpriteComponent>("assets/borderleft.png");
    wall1.addComponent<ColliderComponent>("wall");
    wall1.addGroup(groupBorder);

    // стенка справа 
    auto& wall2(manager.addEntity());
    wall2.addComponent<TransformComponent>(928.0f, 32.0f, 566, 32, 1);
    wall2.addComponent<SpriteComponent>("assets/borderright.png");
    wall2.addComponent<ColliderComponent>("wall");
    wall2.addGroup(groupBorder);

    // стенка снизу
    wall.addComponent<TransformComponent>(32.0f, 598.0f, 32, 896, 1);
    wall.addComponent<SpriteComponent>("assets/borderdown.png");
    wall.addComponent<ColliderComponent>("wall");

    // 4 квадратика по краям
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
    // 

    hole.addComponent<TransformComponent>(800.0f, 500.0f, 40, 40, 1);
    hole.addComponent<SpriteComponent>("assets/hole.png");
    hole.addComponent<ColliderComponent>("hole");
    hole.addGroup(groupHole);
  



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
    case SDL_KEYDOWN: 
        if (event.key.keysym.sym == SDLK_ESCAPE) 
        { 
            isRunning = false; 
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



auto& tiles(manager.getGroup(groupMap));
auto& balls(manager.getGroup(groupBall));
auto& holes(manager.getGroup(groupHole));
auto& borders(manager.getGroup(groupBorder));
auto& boosters(manager.getGroup(groupBooster));


void Game::update()
{
    manager.refresh();
    manager.update();

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
                    cc->tag != "hole" && cc->tag != "dirt" && cc->tag != "sand"
                    && cc->tag != "boosterright" && cc->tag != "ice")
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
                            newPlayer.getComponent<TransformComponent>().position.y += intersectY; // Смещение
                        }
                        else {
                            std::cout << "COLLISION FROM BOTTOM\n";
                            newPlayer.getComponent<TransformComponent>().velocity.y *= -1;
                            newPlayer.getComponent<TransformComponent>().acceleration.y *= -1;
                            newPlayer.getComponent<TransformComponent>().position.y -= intersectY; // Смещение
                        }
                    }
                    else {
                        if (deltaX > 0) {
                            std::cout << "COLLISION FROM LEFT\n";
                            newPlayer.getComponent<TransformComponent>().velocity.x *= -1;
                            newPlayer.getComponent<TransformComponent>().acceleration.x *= -1;
                            newPlayer.getComponent<TransformComponent>().position.x += intersectX; // Смещение
                        }
                        else {
                            std::cout << "COLLISION FROM RIGHT\n";
                            newPlayer.getComponent<TransformComponent>().velocity.x *= -1;
                            newPlayer.getComponent<TransformComponent>().acceleration.x *= -1;
                            newPlayer.getComponent<TransformComponent>().position.x -= intersectX; // Смещение
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
                    if (holeSound) {
                        Mix_PlayChannel(-1, holeSound, 0);
                    }
                    for (auto& tile : tiles)
                    {
                        tile->destroy();
                    }
                    for (auto& hole : holes) hole->destroy();
                    tiles.clear();
                    holes.clear();
                   
                    currentLevel++;
                    loadLevel(currentLevel);
                }
                else if (cc->tag == "dirt")
                {
                    newPlayer.getComponent<TransformComponent>().velocity.x *= 0.98;
                    newPlayer.getComponent<TransformComponent>().velocity.y *= 0.98;
                }

                else if (cc->tag == "sand")
                {
                    newPlayer.getComponent<TransformComponent>().velocity.x *= 0.95;
                    newPlayer.getComponent<TransformComponent>().velocity.y *= 0.95;
                }

                else if (cc->tag == "ice")
                {
                    
                }
               
            }
    }

    elapsedTime = (SDL_GetTicks() - startTime) / 1000.0f; // Конвертируем в секунды
}



void Game::render()
{
    SDL_RenderClear(renderer);
 

    for (auto& tile : tiles)
    {
        tile->draw();
    }

    for (auto& border : borders)
    {
        border->draw();
    }
    
    for (auto& hole : holes)
    {
        hole->draw();
    }
    for (auto& booster : boosters)
    {
        booster->draw();
    }
    for (auto& ball : balls)
    {
        ball->draw();
    }

    TTF_Font* font = TTF_OpenFont("assets/font/font.ttf", 30); // Путь к вашему шрифту, размер шрифта
    if (!font) {
        std::cerr << "Не удалось загрузить шрифт: " << TTF_GetError() << std::endl;
    }
    std::string strokesText = "Strokes: " + std::to_string(newPlayer.getComponent<BallMechanic>().strokes);
    renderText(renderer, font, strokesText, 33, 0); // Текст в левом верхнем углу

    std::string levelText = "Level: " + std::to_string(currentLevel);
    renderText(renderer, font, levelText, 900 - 60, 0); 

    std::string nameText = "CursGolf";
    renderText(renderer, font, nameText, 900 - 480, 0);

    std::string escText = "esc for exit";
    renderText(renderer, font, escText, 32, 596);

    std::ostringstream timerStream;
    timerStream.precision(1);
    timerStream << std::fixed << elapsedTime;
    std::string timerText = "Time: " + timerStream.str() + "s";
    renderText(renderer, font, timerText, 620, 0);

    SDL_RenderPresent(renderer);

}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    Mix_CloseAudio();
    SDL_Quit();
    std::cout << "You've exited the game" << std::endl;
}

void Game::AddTile(int srcX, int srcY, int xpos, int ypos)
{
    auto& tile(manager.addEntity());
    tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, mapfile);
    tile.addGroup(groupMap);
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



void Game::loadLevel(int level)
{

    
}