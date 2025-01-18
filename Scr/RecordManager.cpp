#include "RecordManager.h"
#include <sstream>
#include <iostream>


RecordManager::RecordManager() 
{
}



RecordManager::~RecordManager()
{
    TTF_CloseFont(font);
}

void RecordManager::init(const std::string& recordFilePath, const char* title, int xpos, int ypos, int width, int height,  bool fullscreen)
{

    int flags = 0;
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        TTF_Init();
        recordsWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        recordsRenderer = SDL_CreateRenderer(recordsWindow, -1, 0);
        if (recordsRenderer)
        {
            SDL_SetRenderDrawColor(recordsRenderer, 200, 255, 200, 255);
        }
        font = TTF_OpenFont("assets/font/font.ttf", 30);
        if (!font)
        {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        }
        running = true;
        recordsFile = recordFilePath;

        loadRecord();

    }
    
}

void RecordManager::loadRecord()
{
    std::ifstream file(recordsFile);
    if (file.is_open())
    {
        int level, bestResult;
        while (file >> level >> bestResult)
        {
            bestRecords[level] = bestResult;
        }
        file.close();
    }
    else
    {
        std::cerr << "Record file not found. Starting with empty records." << std::endl;
    }
}

void RecordManager::saveRecord()
{
    std::ofstream file(recordsFile);
    if (file.is_open())
    {
        for (const auto& pair : bestRecords)
        {
            file << pair.first << " " << pair.second << std::endl;
        }
        file.close();
    }
    else
    {
        std::cerr << "Failed to save records to file." << std::endl;
    }
}

void RecordManager::updateRecord(int level, int currentScore)
{
    if (currentScore > bestRecords[level])
    {
        bestRecords[level] = currentScore;
        saveRecord();
    }
}



void RecordManager::render(int startX, int startY)
{

    SDL_RenderClear(recordsRenderer);

    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Color whiteColor = { 0, 0, 0 };
    SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "CursGolf", whiteColor);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(recordsRenderer, titleSurface);
    
    SDL_Rect titleRect;
    titleRect.x = 250;
    titleRect.y = 40;
    titleRect.w = titleSurface->w;
    titleRect.h = titleSurface->h;

    int lineSkip = TTF_FontLineSkip(font);

    SDL_FreeSurface(titleSurface);
    SDL_RenderCopy(recordsRenderer, titleTexture, NULL, &titleRect);
    SDL_DestroyTexture(titleTexture);
    
   // считываем рекорды из файла и выводим их на экран
    for (const auto& pair : bestRecords)
    {
        std::stringstream ss;
        ss << "Level " << pair.first << ": " << pair.second;
        std::string textStr = ss.str(); 
        const char* text = textStr.c_str(); 
        

        SDL_Surface* surfaceText = TTF_RenderText_Solid(font, text, textColor);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(recordsRenderer, surfaceText);
        
        
        if (texture)
        {
            SDL_Rect destRect = { startX, startY, 0, 0 };
            SDL_QueryTexture(texture, nullptr, nullptr, &destRect.w, &destRect.h);
            SDL_RenderCopy(recordsRenderer, texture, nullptr, &destRect);
            SDL_DestroyTexture(texture);
            startY += lineSkip;
        }
    }

    SDL_RenderPresent(recordsRenderer);

}

void RecordManager::handleEvents()
{
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            selectedOption = -1; // Устанавливаем опцию -1, чтобы отличить закрытие окна
            running = false;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
  
            case SDLK_RETURN:
                running = false;
                break;
            }
        }
    }
    
}
