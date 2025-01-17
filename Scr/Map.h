#pragma once
#include <string>
#include "Game.h"

class Map {
public:
	Map(Manager& manager, const char* mfp, int ms, int ts);
	~Map();

	void LoadMap(std::string path, int sizeX, int sizeY);
	void AddTile(int srcX, int srcY, int xpos, int ypos);

private:
	Manager& manager;
	const char* mapFilePath;
	int mapScale;
	int tileSize;
	int scaledSize;
};