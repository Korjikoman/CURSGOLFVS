#include "Map.h"
#include "Game.h"
#include <fstream>

Map::~Map()
{

}


Map::Map()
{
}


// ��������� �����
void Map::LoadMap(std::string path, int sizeX, int sizeY)
{
	char tile;
	std::fstream mapFile;

	mapFile.open(path); // ��������� ���� � ������


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
//    mapFile.open(path); // ��������� ���� � ������
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
//            mapFile >> tileValue; // ������ ����� �� �����
//
//            // �����������, ��� tileValue ������������� tile � ����� �����
//            srcY = tileValue * 32;
//            srcX = tileValue * 32;
//
//            std::cout << tileValue << " "; // ������� �������� ��� �������
//
//            Game::AddTile(srcX, srcY, x * 16, y * 16);
//
//            // ���������� ����������� (��������, ������� ��� �������)
//            mapFile.ignore();
//        }
//        std::cout << "\n";
//    }
//
//    mapFile.close();
//}