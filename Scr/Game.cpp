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
bool Game::win = false; // ������������� ���������� win
int currentLevel = 1;

Mix_Chunk* holeSound = nullptr;
Mix_Chunk* budaSound = nullptr;
Uint32 startTime; // ����� ������ �������
float elapsedTime; // ��������� ����� � ��������


const char* mapfile = "assets/2tileset.png";


auto& newPlayer(manager.addEntity());
auto& hole(manager.addEntity());
auto& flag(manager.addEntity());

auto& wall(manager.addEntity());
auto& box(manager.addEntity());


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
    map = new Map("assets/2tileset.png", 1, 32);

    startTime = SDL_GetTicks(); // ���������� ����� ������ � �������������
    elapsedTime = 0; // �������������� ��������� �����


    // ecs implementation

    map->LoadMap("assets/map1.map", 30, 19);

    newPlayer.addComponent<TransformComponent>(100.0f, 100.0f, 32, 32, 1);
    newPlayer.addComponent<SpriteComponent>("assets/ball.png");
    newPlayer.addComponent<BallMechanic>();
    newPlayer.addComponent<ColliderComponent>("ball");
    newPlayer.addGroup(groupBall);

    // ������� ������
    wall.addComponent<TransformComponent>(32.0f, 0.0f, 32, 896, 1);
    wall.addComponent<SpriteComponent>("assets/borderup.png");
    wall.addComponent<ColliderComponent>("wall");
    wall.addGroup(groupColliders);

    // ������ ����� 
    auto& wall2(manager.addEntity());
    wall2.addComponent<TransformComponent>(0.0f, 32.0f, 566, 32, 1);
    wall2.addComponent<SpriteComponent>("assets/borderleft.png");
    wall2.addComponent<ColliderComponent>("wall");
    wall2.addGroup(groupColliders);

    // ������ ������ 
    auto& wall3(manager.addEntity());
    wall3.addComponent<TransformComponent>(928.0f, 32.0f, 566, 32, 1);
    wall3.addComponent<SpriteComponent>("assets/borderright.png");
    wall3.addComponent<ColliderComponent>("wall");
    wall3.addGroup(groupColliders);

    // ������ �����
    auto& wall4(manager.addEntity());

    wall4.addComponent<TransformComponent>(32.0f, 598.0f, 32, 896, 1);
    wall4.addComponent<SpriteComponent>("assets/borderdown.png");
    wall4.addComponent<ColliderComponent>("wall");
    wall4.addGroup(groupColliders);

    // 4 ���������� �� �����
    auto& wall5(manager.addEntity());

    wall5.addComponent<TransformComponent>(0.0f, 0.0f, 32, 32, 1);
    wall5.addComponent<SpriteComponent>("assets/border.png");
    wall5.addComponent<ColliderComponent>("wall");
    wall5.addGroup(groupColliders);

    auto& wall6(manager.addEntity());

    wall6.addComponent<TransformComponent>(928.0f, 598.0f, 32, 32, 1);
    wall6.addComponent<SpriteComponent>("assets/border.png");
    wall6.addComponent<ColliderComponent>("wall");
    wall6.addGroup(groupColliders);

    auto& wall7(manager.addEntity());

    wall7.addComponent<TransformComponent>(928.0f, 0.0f, 32, 32, 1);
    wall7.addComponent<SpriteComponent>("assets/border.png");
    wall7.addComponent<ColliderComponent>("wall");
    wall7.addGroup(groupColliders);


    auto& wall8(manager.addEntity());
    wall8.addComponent<TransformComponent>(0.0f, 598.0f, 32, 32, 1);
    wall8.addComponent<SpriteComponent>("assets/border.png");
    wall8.addComponent<ColliderComponent>("wall");
    wall8.addGroup(groupColliders);

    // 
    
    hole.addComponent<TransformComponent>(850.0f, 80.0f, 40, 40, 1);
    hole.addComponent<SpriteComponent>("assets/hole.png");
    hole.addComponent<ColliderComponent>("hole");
    hole.addGroup(groupColliders);
    
    
    flag.addComponent<TransformComponent>(852.0f, 30.0f, 100, 50, 1);
    flag.addComponent<SpriteComponent>("assets/flag.png");
    flag.addGroup(groupFlag);



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



auto& tiles(manager.getGroup(Game::groupMap));
auto& balls(manager.getGroup(Game::groupBall));
auto& holes(manager.getGroup(Game::groupHole));
auto& borders(manager.getGroup(Game::groupBorder));
auto& boosters(manager.getGroup(Game::groupBooster));
auto& flags(manager.getGroup(Game::groupFlag));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& walls(manager.getGroup(Game::groupBorder));



void Game::update()
{

    SDL_Rect ballCol = newPlayer.getComponent<ColliderComponent>().collider;
    Vector2D ballPos = newPlayer.getComponent<TransformComponent>().position;


    manager.refresh();
    manager.update();
   
    bool collisionProcessed = false;
    for (auto c : colliders)
    {
        if (collisionProcessed) break;

        SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
        std::string tag = c->getComponent<ColliderComponent>().tag;
        if (Collision::AABB(newPlayer.getComponent<ColliderComponent>(), c->getComponent<ColliderComponent>()))
        {
            collisionProcessed = true;
  
            // �������� ����������
            SDL_Rect* ball = &newPlayer.getComponent<ColliderComponent>().collider;
            SDL_Rect* entity = &c->getComponent<ColliderComponent>().collider;

            // ����������, � ����� ������� ��������� ������������
            int ballCenterX = ball->x + ball->w / 2;
            int ballCenterY = ball->y + ball->h / 2;
            int entityCenterX = entity->x + entity->w / 2;
            int entityCenterY = entity->y + entity->h / 2;

            int deltaX = ballCenterX - entityCenterX;
            int deltaY = ballCenterY - entityCenterY;

            int intersectX = (ball->w / 2 + entity->w / 2) - abs(deltaX);
            int intersectY = (ball->h / 2 + entity->h / 2) - abs(deltaY);

            if (ball->x < entity->x + entity->w &&
                ball->x + ball->w > entity->x &&
                ball->y < entity->y + entity->h &&
                ball->y + ball->h > entity->y &&
                tag != "hole" && tag != "sand" && tag != "dirt"
                && tag != "boosterright" && tag != "ice" && tag != "UPPDOWNER" 
                && tag != "LEFTRIGHTER" && tag != "corUPLEFT"
                && tag != "corUPRIGHT" && tag != "corDOWNLEFT"
                && tag != "corDOWNRIGHT")
            {

                // ���� ����������� �� X ������, ��� �� Y, �� ������������ ��������� ������ ��� �����
                if (intersectX > intersectY) {
                    if (deltaY > 0) {
                        std::cout << "COLLISION FROM TOP\n";
                        newPlayer.getComponent<TransformComponent>().position.y += intersectY; // �������� �����
                        newPlayer.getComponent<TransformComponent>().velocity.y *= -1;
                        newPlayer.getComponent<TransformComponent>().acceleration.y *= -1;
                    }
                    else {
                        std::cout << "COLLISION FROM BOTTOM\n";
                        newPlayer.getComponent<TransformComponent>().position.y -= intersectY; // �������� ����
                        newPlayer.getComponent<TransformComponent>().velocity.y *= -1;
                        newPlayer.getComponent<TransformComponent>().acceleration.y *= -1;
                    }
                }
                else {
                    if (deltaX > 0) {
                        std::cout << "COLLISION FROM LEFT\n";
                        newPlayer.getComponent<TransformComponent>().position.x += intersectX; // �������� ������
                        newPlayer.getComponent<TransformComponent>().velocity.x *= -1;
                        newPlayer.getComponent<TransformComponent>().acceleration.x *= -1;
                    }
                    else {
                        std::cout << "COLLISION FROM RIGHT\n";
                        newPlayer.getComponent<TransformComponent>().position.x -= intersectX; // �������� �����
                        newPlayer.getComponent<TransformComponent>().velocity.x *= -1;
                        newPlayer.getComponent<TransformComponent>().acceleration.x *= -1;
                    }
                }

            }
            
            else if (tag == "dirt")
            {
                newPlayer.getComponent<TransformComponent>().velocity.x *= 0.98;
                newPlayer.getComponent<TransformComponent>().velocity.y *= 0.98;
            }

            else if (tag == "sand")
            {
                newPlayer.getComponent<TransformComponent>().velocity.x *= 0.95;
                newPlayer.getComponent<TransformComponent>().velocity.y *= 0.95;
            }

            else if (tag == "ice")
            {

            }

            else if (ball->x < entity->x + entity->w &&
                ball->x + ball->w > entity->x &&
                ball->y < entity->y + entity->h &&
                ball->y + ball->h > entity->y && (tag == "UPPDOWNER"))
            {

                // ���� ����������� �� X ������, ��� �� Y, �� ������������ ��������� ������ ��� �����
                
                 if (deltaY > 0) {
                     std::cout << "COLLISION FROM TOP\n";
                     newPlayer.getComponent<TransformComponent>().position.y += intersectY; // �������� �����
                     newPlayer.getComponent<TransformComponent>().velocity.y *= -1;
                     newPlayer.getComponent<TransformComponent>().acceleration.y *= -1;
                 }
                 else {
                     std::cout << "COLLISION FROM BOTTOM\n";
                     newPlayer.getComponent<TransformComponent>().position.y -= intersectY; // �������� ����
                     newPlayer.getComponent<TransformComponent>().velocity.y *= -1;
                     newPlayer.getComponent<TransformComponent>().acceleration.y *= -1;
                 }
            }

            else if (ball->x < entity->x + entity->w &&
                ball->x + ball->w > entity->x &&
                ball->y < entity->y + entity->h &&
                ball->y + ball->h > entity->y && (tag == "LEFTRIGHTER"))
            {

                // ���� ����������� �� X ������, ��� �� Y, �� ������������ ��������� ������ ��� �����

                 if (deltaX > 0) {
                     std::cout << "COLLISION FROM LEFT\n";
                     newPlayer.getComponent<TransformComponent>().position.x += intersectX; // �������� ������
                     newPlayer.getComponent<TransformComponent>().velocity.x *= -1;
                     newPlayer.getComponent<TransformComponent>().acceleration.x *= -1;
                 }
                 else {
                     std::cout << "COLLISION FROM RIGHT\n";
                     newPlayer.getComponent<TransformComponent>().position.x -= intersectX; // �������� �����
                     newPlayer.getComponent<TransformComponent>().velocity.x *= -1;
                     newPlayer.getComponent<TransformComponent>().acceleration.x *= -1;
                 }
            } 
            else if (ball->x < entity->x + entity->w &&
                ball->x + ball->w > entity->x &&
                ball->y < entity->y + entity->h &&
                ball->y + ball->h > entity->y && (tag == "corDOWNRIGHT"))
            {

                 // ���� ����������� �� X ������, ��� �� Y, �� ������������ ��������� ������ ��� �����
                 if (intersectX > intersectY) {
                     if (deltaY > 0) {
                         std::cout << "COLLISION FROM TOP\n";
                         newPlayer.getComponent<TransformComponent>().position.y += intersectY; // �������� �����
                         newPlayer.getComponent<TransformComponent>().velocity.y *= -1;
                         newPlayer.getComponent<TransformComponent>().acceleration.y *= -1;
                     }
                 }
                 else {
                     if (deltaX > 0) {
                         std::cout << "COLLISION FROM LEFT\n";
                         newPlayer.getComponent<TransformComponent>().position.x += intersectX; // �������� ������
                         newPlayer.getComponent<TransformComponent>().velocity.x *= -1;
                         newPlayer.getComponent<TransformComponent>().acceleration.x *= -1;
                     }
                 }
            }
            else if (ball->x < entity->x + entity->w &&
                ball->x + ball->w > entity->x &&
                ball->y < entity->y + entity->h &&
                ball->y + ball->h > entity->y && (tag == "corDOWNLEFT"))
            {

                 // ���� ����������� �� X ������, ��� �� Y, �� ������������ ��������� ������ ��� �����
                 if (intersectX > intersectY) {
                     if (deltaY > 0) {
                         std::cout << "COLLISION FROM TOP\n";
                         newPlayer.getComponent<TransformComponent>().position.y += intersectY; // �������� �����
                         newPlayer.getComponent<TransformComponent>().velocity.y *= -1;
                         newPlayer.getComponent<TransformComponent>().acceleration.y *= -1;
                     }
                 }
                 else {
                     std::cout << "COLLISION FROM RIGHT\n";
                     newPlayer.getComponent<TransformComponent>().position.x -= intersectX; // �������� �����
                     newPlayer.getComponent<TransformComponent>().velocity.x *= -1;
                     newPlayer.getComponent<TransformComponent>().acceleration.x *= -1;
                 }
            }

            else if (ball->x < entity->x + entity->w &&
                ball->x + ball->w > entity->x &&
                ball->y < entity->y + entity->h &&
                ball->y + ball->h > entity->y && (tag == "corUPRIGHT"))
            {

                 // ���� ����������� �� X ������, ��� �� Y, �� ������������ ��������� ������ ��� �����
                 if (intersectX > intersectY) {
                     std::cout << "COLLISION FROM BOTTOM\n";
                     newPlayer.getComponent<TransformComponent>().position.y -= intersectY; // �������� ����
                     newPlayer.getComponent<TransformComponent>().velocity.y *= -1;
                     newPlayer.getComponent<TransformComponent>().acceleration.y *= -1;
                 }
                 else {
                     if (deltaX > 0) {
                         std::cout << "COLLISION FROM LEFT\n";
                         newPlayer.getComponent<TransformComponent>().position.x += intersectX; // �������� ������
                         newPlayer.getComponent<TransformComponent>().velocity.x *= -1;
                         newPlayer.getComponent<TransformComponent>().acceleration.x *= -1;
                     }
                 }
            }

            else if (ball->x < entity->x + entity->w &&
                ball->x + ball->w > entity->x &&
                ball->y < entity->y + entity->h &&
                ball->y + ball->h > entity->y && (tag == "corUPLEFT"))
            {

                 // ���� ����������� �� X ������, ��� �� Y, �� ������������ ��������� ������ ��� �����
                    if (intersectX > intersectY) {
                        std::cout << "COLLISION FROM BOTTOM\n";
                        newPlayer.getComponent<TransformComponent>().position.y -= intersectY; // �������� ����
                        newPlayer.getComponent<TransformComponent>().velocity.y *= -1;
                        newPlayer.getComponent<TransformComponent>().acceleration.y *= -1;
                    }
                    else {
                        std::cout << "COLLISION FROM RIGHT\n";
                        newPlayer.getComponent<TransformComponent>().position.x -= intersectX; // �������� �����
                        newPlayer.getComponent<TransformComponent>().velocity.x *= -1;
                        newPlayer.getComponent<TransformComponent>().acceleration.x *= -1;
                    }
            }
            else if (tag == "hole")
            {
                newPlayer.getComponent<BallMechanic>().strokes = 0;
                if (holeSound) {
                    Mix_PlayChannel(-1, holeSound, 0);
                }
                for (auto& tile : tiles)
                {
                    tile->destroy();
                }
                for (auto& col : colliders) {
                    SDL_Rect coldestroy = col->getComponent<ColliderComponent>().collider;
                    std::string tagdestroy = col->getComponent<ColliderComponent>().tag;
                    if (tagdestroy != "wall" && tagdestroy != "hole") col->destroy();
                }
                if (currentLevel == 1)
                {
                    map->LoadMap("assets/map2.map", 30, 19);
                }

                currentLevel++;

                if (currentLevel == 2)
                {
                    newPlayer.getComponent<TransformComponent>().velocity.x = 0;
                    newPlayer.getComponent<TransformComponent>().velocity.y = 0;
                    newPlayer.getComponent<TransformComponent>().position.x = 50.0f;
                    newPlayer.getComponent<TransformComponent>().position.y = 550.0f;
                    hole.getComponent<TransformComponent>().position.x = 870.0f;
                    hole.getComponent<TransformComponent>().position.y = 500.0f;
                } 
                if (currentLevel == 3)
                {
                    newPlayer.getComponent<TransformComponent>().velocity.x = 0;
                    newPlayer.getComponent<TransformComponent>().velocity.y = 0;
                    newPlayer.getComponent<TransformComponent>().position.x = 50.0f;
                    newPlayer.getComponent<TransformComponent>().position.y = 50.0f;
                    hole.getComponent<TransformComponent>().position.x = 870.0f;
                    hole.getComponent<TransformComponent>().position.y = 500.0f;
                }
            }
        } 
    }
    elapsedTime = (SDL_GetTicks() - startTime) / 1000.0f; // ������������ � �������
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
    
    for (auto& c : colliders)
    {
        c->draw();
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
    for (auto& flag : flags)
    {
        flag->draw();
    }

    TTF_Font* font = TTF_OpenFont("assets/font/font.ttf", 30); // ���� � ������ ������, ������ ������
    if (!font) {
        std::cerr << "�� ������� ��������� �����: " << TTF_GetError() << std::endl;
    }
    std::string strokesText = "Strokes: " + std::to_string(newPlayer.getComponent<BallMechanic>().strokes);
    renderText(renderer, font, strokesText, 33, 0); // ����� � ����� ������� ����

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



void Game::renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y)
{
    SDL_Color textColor = { 0, 0, 0, 255 }; // ������ ���� ������

    // ������ ����������� � �������
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if (!surface) {
        std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
        return;
    }

    // ������ �������� �� �����������
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    // ������������� ������ ������
    SDL_Rect destRect = { x, y, surface->w, surface->h };

    // ����������� �����������
    SDL_FreeSurface(surface);

    // ���������� ��������
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);

    // ������� �������� ����� ���������
    SDL_DestroyTexture(texture);
}

