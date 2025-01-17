
#include "Game.h"
#include "menu.h"
#include "InfoWindow.h"

using namespace std;

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 630;

SDL_Window *window = nullptr;
SDL_Surface *screen = NULL;
SDL_Renderer *renderer = nullptr;

Game *game = nullptr;
Menu* menu = nullptr;
InfoWindow* infoWindow = nullptr;

int main(int, char **)
{
    const int FPS = 144;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    bool gameRunning = true;



    while (gameRunning) {
        menu = new Menu();
        menu->init("CursGolf", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 350, false);

        while (menu->isRunning()) {
            menu->handleEvents();
            menu->render();
        }

        int selectedOption = menu->getSelectedOption();
        delete menu; // Уничтожаем меню перед началом игры

        if (selectedOption == 0) { // Если выбрана "Игра"
            game = new Game();
            game->init("CursGolf", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);

            while (game->running()) {
                frameStart = SDL_GetTicks();

                game->handleEvents();
                game->update();
                game->render();

                frameTime = SDL_GetTicks() - frameStart;

                if (frameDelay > frameTime) {
                    SDL_Delay(frameDelay - frameTime);
                }
            }

            game->clean();
            delete game;
        }
        else if (selectedOption == 2) { // Если выбрана "Info"
            infoWindow = new InfoWindow();
            infoWindow->init("Game Info", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 300, false);

            while (infoWindow->isRunning()) {
                infoWindow->handleEvents();
                infoWindow->render();
            }

            delete infoWindow;
        }
        else if (selectedOption == -1 || selectedOption == 3) { // Если выбран "Выход" или окно закрыто
            gameRunning = false;
        }
    }
    return 0;
}