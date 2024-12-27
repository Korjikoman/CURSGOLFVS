#pragma once
#include "Components.h"
#include "../Vector2D.h"

class TransformComponent : public Component
{
public:

	Vector2D position;
	Vector2D velocity;
	
	float acceleration;
	float speed;

	TransformComponent() {
		position.x = 200.0f;
		position.y = 200.0f;
	}

	TransformComponent(float x, float y)
	{
		position.x = x;
		position.y = y;

	}


	void update() override
	{
		if (speed > 0.0f)
		{
			position.x += velocity.x * speed;
			position.y += velocity.y * speed;
			std::cout << "Position (" << position.x << "; " << position.y << " )\n";
			std::cout << "VELOCITY (" << velocity.x << "; " << velocity.y << " )\n";
			std::cout << "Speed" << speed<< std::endl;

			speed -= acceleration * 2;
		}
		else
		{
			speed = 0;
			position.x += velocity.x;
			position.y += velocity.y;
		}
	}

	void setPos(int x, int y)
	{
		position.x = x;
		position.y = y;
	}

	void init() override
	{
		velocity.x = 0;
		velocity.y = 0;
	}
};