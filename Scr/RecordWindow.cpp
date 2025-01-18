#include "RecordWindow.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

RecordWindow::RecordWindow() : running(false) {}

RecordWindow::~RecordWindow() {
    SDL_DestroyWindow(recordWindow);
    SDL_DestroyRenderer(recordRenderer);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}

void RecordWindow::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        TTF_Init();
        recordWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        recordRenderer = SDL_CreateRenderer(recordWindow, -1, 0);

        if (recordRenderer) {
            SDL_SetRenderDrawColor(recordRenderer, 255, 255, 255, 255);
        }

        font = TTF_OpenFont("assets/font/font.ttf", 20);
        if (!font) {
            std::cerr << "�� ������� ��������� �����: " << TTF_GetError() << std::endl;
        }

        running = true;
    }
    else {
        running = false;
    }
}

void RecordWindow::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT ||
            (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) ||
            (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            running = false;
        }
    }
}

void RecordWindow::render() {
    SDL_RenderClear(recordRenderer);
    SDL_Color textColor = { 0, 0, 0 };

    // ��������� ������� �� �����
    std::vector<std::string> records = loadRecords("assets/records/records.txt");

    // ���������
    std::string header = "Level Records:";
    std::string minus = "----------------------------------------";
    std::vector<std::string> lines;
    lines.push_back(header);
    lines.push_back(minus);

    // ��������� ������� � ������ ��� �����������
    for (size_t i = 0; i < records.size(); ++i) {
        std::string record = "Level " + std::to_string(i + 1) + ": " + records[i] + " strokes";
        lines.push_back(record);
    }

    // ���������� �����
    int yOffset = 20; // ��������� ������� �� Y
    for (const auto& str : lines) {
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, str.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(recordRenderer, textSurface);

        SDL_Rect textRect = { 20, yOffset, textSurface->w, textSurface->h };
        SDL_FreeSurface(textSurface);

        SDL_RenderCopy(recordRenderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);

        yOffset += 30; // �������� �� Y ��� ��������� ������
    }

    SDL_RenderPresent(recordRenderer);
}

bool RecordWindow::isRunning() const {
    return running;
}

std::vector<std::string> RecordWindow::loadRecords(const std::string& filepath) {
    std::vector<std::string> records;
    std::ifstream file(filepath);

    if (!file) {
        std::cerr << "�� ������� ������� ���� � ���������: " << filepath << std::endl;
        return records;
    }

    std::string line;
    while (std::getline(file, line)) {
        records.push_back(line);
    }

    file.close();
    return records;
}
