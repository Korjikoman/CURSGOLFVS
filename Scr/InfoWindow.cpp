#include "InfoWindow.h"
#include <iostream>
#include <vector>
#include <sstream>

InfoWindow::InfoWindow() : running(false) {}

InfoWindow::~InfoWindow() {
    SDL_DestroyWindow(infoWindow);
    SDL_DestroyRenderer(infoRenderer);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}

void InfoWindow::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        TTF_Init();
        infoWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        infoRenderer = SDL_CreateRenderer(infoWindow, -1, 0);

        if (infoRenderer) {
            SDL_SetRenderDrawColor(infoRenderer, 255, 255, 255, 255);
        }

        font = TTF_OpenFont("assets/font/font.ttf", 20);
        if (!font) {
            std::cerr << "Не удалось загрузить шрифт: " << TTF_GetError() << std::endl;
        }

        running = true;
    }
    else {
        running = false;
    }
}

void InfoWindow::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)) {
            running = false;
        }
    }
}

void InfoWindow::render() {
    SDL_RenderClear(infoRenderer);
    SDL_Color textColor = { 0, 0, 0 };

    // Текст для отображения
    const char* infoText = "Welcome to the CursGolf!\n- Use arrow keys to navigate\n- Control the ball with your mouse\n- Try to make fewer strokes\n- Try to complete the level in less time\n- Use enter to exit\nEnjoy!\n----------------------------------------\n         made by Andrey and Stas from PI-32";
    std::vector<std::string> lines;
    std::stringstream textStream(infoText);
    std::string line;

    // Разбиваем текст на строки по символу '\n'
    while (std::getline(textStream, line, '\n')) {
        lines.push_back(line);
    }

    int yOffset = 20; // Начальная позиция по Y
    for (const auto& str : lines) {
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, str.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(infoRenderer, textSurface);

        SDL_Rect textRect = { 20, yOffset, textSurface->w, textSurface->h };
        SDL_FreeSurface(textSurface);

        SDL_RenderCopy(infoRenderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);

        yOffset += 30; // Смещение по Y для следующей строки
    }

    SDL_RenderPresent(infoRenderer);
}


bool InfoWindow::isRunning() const {
    return running;
}
