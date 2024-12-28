#pragma once
#include "Components.h"
#include "../Vector2D.h"

class TransformComponent : public Component
{
public:

	Vector2D position;
	Vector2D velocity;
	Vector2D acceleration;

	int speed;

	int height = 32;
	int width = 32;
	int scale = 1;


	TransformComponent() 
	{
		position.Zero();
	}

	TransformComponent(int sc) 
	{
		position.Zero();
		scale = sc;
	}
	TransformComponent(float x, float y)
	{
		position.x = x;
		position.y = y;

	}

	TransformComponent(float x, float y, int h, int w, int sc)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}

	void update() override
	{
		if (abs(velocity.x) > 0.002f && abs(velocity.y) > 0.002f)
		{
			position.x += velocity.x;
			position.y += velocity.y;
			std::cout << "Position (" << position.x << "; " << position.y << " )\n";
			std::cout << "VELOCITY (" << velocity.x << "; " << velocity.y << " )\n";
			velocity.x += acceleration.x;
			velocity.y += acceleration.y;
		}
		
			
	}


	void setPos(int x, int y)
	{
		position.x = x;
		position.y = y;
	}

	void init() override
	{
		velocity.Zero();
	}
};