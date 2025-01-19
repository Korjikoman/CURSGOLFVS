#include "Menu.h"
#include <iostream>

Menu::Menu() : running(false), selectedOption(0) {}

Menu::~Menu() {
    SDL_DestroyWindow(menuWindow);
    SDL_DestroyRenderer(menuRenderer);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}

void Menu::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        TTF_Init();
        menuWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        menuRenderer = SDL_CreateRenderer(menuWindow, -1, 0);

        if (menuRenderer) {
            SDL_SetRenderDrawColor(menuRenderer, 200, 255, 200, 255);
        }

        font = TTF_OpenFont("assets/font/font.ttf", 30);
        if (!font) {
            std::cerr << "Не удалось загрузить шрифт: " << TTF_GetError() << std::endl;
        }

        running = true;
        menuOptions = { "Game", "Records", "Info", "Exit" };
    }
    else {
        running = false;
    }
}

void Menu::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            selectedOption = -1; // Устанавливаем опцию -1, чтобы отличить закрытие окна
            running = false;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_UP:
                selectedOption = (selectedOption > 0) ? selectedOption - 1 : menuOptions.size() - 1;
                break;
            case SDLK_DOWN:
                selectedOption = (selectedOption < menuOptions.size() - 1) ? selectedOption + 1 : 0;
                break;
            case SDLK_RETURN:
                running = false;
                break;
            }
        }
    }
}


void Menu::render() {
    SDL_RenderClear(menuRenderer);
    // Рендерим заголовок "CursGolf"
    SDL_Color whiteColor = { 0, 0, 0 };
    SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "CursGolf", whiteColor);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(menuRenderer, titleSurface);

    SDL_Rect titleRect;
    titleRect.x = 250;
    titleRect.y = 40;
    titleRect.w = titleSurface->w;
    titleRect.h = titleSurface->h;

    SDL_FreeSurface(titleSurface);
    SDL_RenderCopy(menuRenderer, titleTexture, NULL, &titleRect);
    SDL_DestroyTexture(titleTexture);

    for (int i = 0; i < menuOptions.size(); ++i) {
        SDL_Color color = (i == selectedOption) ? SDL_Color{ 0, 150, 0 } : SDL_Color{ 0, 0, 0 };

        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, menuOptions[i].c_str(), color);
        SDL_Texture* message = SDL_CreateTextureFromSurface(menuRenderer, surfaceMessage);

        SDL_Rect messageRect;
        messageRect.x = 100;
        messageRect.y = 100 + i * 50;
        messageRect.w = surfaceMessage->w;
        messageRect.h = surfaceMessage->h;

        SDL_FreeSurface(surfaceMessage);

        SDL_RenderCopy(menuRenderer, message, NULL, &messageRect);
        SDL_DestroyTexture(message);
    }

    SDL_RenderPresent(menuRenderer);
}

