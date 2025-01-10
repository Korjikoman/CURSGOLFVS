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


	int srcX, srcY;


	for (int y = 0; y < sizeY; y++) 
	{
		for (int x = 0; x < sizeX; x++) 
		{
			mapFile.get(tile);
			
			srcY = atoi(&tile) * 32;

			mapFile.get(tile);

			srcX = atoi(&tile) * 32;


			std::cout << tile;
			
			Game::AddTile(srcX, srcY, x*32, y*32
			);
			mapFile.ignore();
		}
		std::cout << "\n";
	}

	mapFile.close();
}

//void Map::LoadMap(std::string path, int sizeX, int sizeY)
//{
//    std::fstream mapFile;
//    mapFile.open(path); // открываем файл с картой
//
//    if (!mapFile.is_open()) {
//        std::cerr << "Failed to open map file!" << std::endl;
//        return;
//    }
//
//    int srcX, srcY;
//    int tileValue;
//
//    for (int y = 0; y < sizeY; y++)
//    {
//        for (int x = 0; x < sizeX; x++)
//        {
//            mapFile >> tileValue; // Читаем число из файла
//
//            // Предположим, что tileValue соответствует tile в вашей карте
//            srcY = tileValue * 32;
//            srcX = tileValue * 32;
//
//            std::cout << tileValue << " "; // Выводим значение для отладки
//
//            Game::AddTile(srcX, srcY, x * 16, y * 16);
//
//            // Пропускаем разделители (например, запятые или пробелы)
//            mapFile.ignore();
//        }
//        std::cout << "\n";
//    }
//
//    mapFile.close();
//}