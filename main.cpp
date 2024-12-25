
#include "Game.h"

using namespace std;

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

SDL_Window *window = nullptr;
SDL_Surface *screen = NULL;
SDL_Renderer *renderer = nullptr;

Game *game = nullptr;
int main(int, char **)
{

    game = new Game();
    game->init("Fuck", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);
    while (game->running())
    {
        game->handleEvents();
        game->update();
        game->render();
    }
    game->clean();
    return 0;
}