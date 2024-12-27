#pragma once
#include "Components.h"
#include "../Vector2D.h"

class TransformComponent : public Component
{
public:

	Vector2D position;
	Vector2D velocity;
	
	int speed;

	Vector2D acceleration;


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
		if (abs(velocity.x) > 0.0f && abs(velocity.y) > 0.0f)
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
		velocity.x = 0;
		velocity.y = 0;
	}
};