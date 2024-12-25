#include "Game.h"

SDL_Texture *playerTex;
SDL_Rect srcR, destR; // sourceRectangle, destinationRectangle


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
    SDL_Surface *tmpSurface = IMG_Load("assets/nigga.png");
    playerTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
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
    counter++;
    destR.h = 64;
    destR.w = 64;
    destR.x = counter;
}

void Game::render()
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, playerTex, NULL, &destR);
    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "You've exited the game" << std::endl;
}
