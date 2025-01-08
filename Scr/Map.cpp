#include "Map.h"
#include "Game.h"
#include <fstream>

Map::~Map()
{

}


Map::Map()
{
}


// Загружаем карту
void Map::LoadMap(std::string path, int sizeX, int sizeY)
{
	char tile;
	std::fstream mapFile;

	mapFile.open(path); // открываем файл с картой

	for (int y = 0; y < sizeY; y++) 
	{
		for (int x = 0; x < sizeX; x++) 
		{
			mapFile.get(tile);
			Game::AddTile(atoi(&tile), x * 32, y * 32);
			mapFile.ignore();
		}

	}

	mapFile.close();
}

