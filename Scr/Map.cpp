#include "Map.h"
#include <fstream>
#include "ECS/ECS.h"
#include "ECS/Components.h"

extern Manager manager;


Map::Map(Manager& manager, const char* mfp, int ms, int ts) : manager(manager), mapFilePath(mfp), mapScale(ms), tileSize(ts)
{
	scaledSize = ms * ts;
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
			
			srcY = atoi(&tile) * tileSize;

			mapFile.get(tile);

			srcX = atoi(&tile) * tileSize;


			std::cout << tile;
			
			AddTile(srcX, srcY, x * scaledSize, y * scaledSize
			);
			mapFile.ignore();
		}
		std::cout << "\n";
	}

	mapFile.ignore();
	std::cout << "\n";
	std::cout << "\n";

	// ���������� ��������
	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(tile);
			std::cout << tile;

			if (tile == '1') {
				Entity& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("terrain", x*scaledSize, y * scaledSize, tileSize * mapScale);
				tcol.addGroup(Game::groupColliders);
				
			}
			if (tile == '2') {
				Entity& tcol2(manager.addEntity());
				tcol2.addComponent<ColliderComponent>("UPPDOWNER", x * scaledSize, y * scaledSize, tileSize * mapScale);
				tcol2.addGroup(Game::groupColliders);

			}
			if (tile == '3') {
				Entity& tcol3(manager.addEntity());
				tcol3.addComponent<ColliderComponent>("LEFTRIGHTER", x * scaledSize, y * scaledSize, tileSize * mapScale);
				tcol3.addGroup(Game::groupColliders);

			}
			if (tile == '4') {
				Entity& tcol4(manager.addEntity());
				tcol4.addComponent<ColliderComponent>("corUPLEFT", x * scaledSize, y * scaledSize, tileSize * mapScale);
				tcol4.addGroup(Game::groupColliders);

			}
			if (tile == '5') {
				auto& tcol5(manager.addEntity());
				tcol5.addComponent<ColliderComponent>("corUPRIGHT", x * scaledSize, y * scaledSize, tileSize * mapScale);
				tcol5.addGroup(Game::groupColliders);

			}
			if (tile == '6') {
				auto& tcol6(manager.addEntity());
				tcol6.addComponent<ColliderComponent>("corDOWNLEFT", x * scaledSize, y * scaledSize, tileSize * mapScale);
				tcol6.addGroup(Game::groupColliders);

			}
			if (tile == '7') {
				auto& tcol7(manager.addEntity());
				tcol7.addComponent<ColliderComponent>("corDOWNRIGHT", x * scaledSize, y * scaledSize, tileSize * mapScale);
				tcol7.addGroup(Game::groupColliders);

			}
			if (tile == '8') {
				auto& tcol8(manager.addEntity());
				tcol8.addComponent<ColliderComponent>("dirt", x * scaledSize, y * scaledSize, tileSize * mapScale);
				tcol8.addGroup(Game::groupColliders);

			}
			if (tile == '9') {
				auto& tcol9(manager.addEntity());
				tcol9.addComponent<ColliderComponent>("sand", x * scaledSize, y * scaledSize, tileSize * mapScale);
				tcol9.addGroup(Game::groupColliders);

			}

			
			mapFile.ignore();
		}
		std::cout << "\n";

	}
	std::cout << "Map generated\n";
	mapFile.close();
}


void Map::AddTile(int srcX, int srcY, int xpos, int ypos)
{
	Entity& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos,tileSize,mapScale, mapFilePath);
	tile.addGroup(Game::groupMap);

}