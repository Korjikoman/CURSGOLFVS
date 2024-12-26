#pragma once
#include "Game.h"

class Map {
public:
	Map();
	~Map();

	void LoadMap(int arr[31][30]);
	void DrawMap();

private:

	SDL_Rect src, dest;
	SDL_Texture* grass;
	SDL_Texture* grass1;
	SDL_Texture* water;
	SDL_Texture* wall;

	int map[31][30];

};