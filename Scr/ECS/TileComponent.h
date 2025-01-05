#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "SDL.h"

class TileComponent : public Component
{
public:
	TransformComponent *transform;
	SpriteComponent* sprite;
	
	SDL_Rect tileRect;
	int tileID;


	char* path;


	TileComponent() = default;
	TileComponent(int x, int y, int w, int h, int id)
	{
		tileRect.x = x;
		tileRect.y = y;
		tileRect.h = h;
		tileRect.w = w;
		tileID = id;

		switch (tileID)
		{
		case 0:
			path = (char*)"assets/grass.png";
			break;
		case 1:
			path = (char*)"assets/wall.png";
			break;
		case 2:
			path = (char*)"assets/water.png";
			break;
		}
	}

	void init() override
	{
		entity->addComponent<TransformComponent>((float)tileRect.x, 
			(float)tileRect.y, tileRect.w, tileRect.h, 1);
		transform = &entity->getComponent<TransformComponent>();

		entity->addComponent<SpriteComponent>(path);
		sprite = &entity->getComponent<SpriteComponent>();
	}

};