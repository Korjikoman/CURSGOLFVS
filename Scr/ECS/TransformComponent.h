#pragma once
#include "Components.h"
#include "../Vector2D.h"

class TransformComponent : public Component
{
public:

	Vector2D position;

	TransformComponent() {
		position.x = 0.0f;
		position.y = 0.0f;
	}

	TransformComponent(float x, float y)
	{
		position.x = x;
		position.y = y;

	}

	int x() { return position.x; }
	int y() { return position.y; }



	void update() override
	{
		position.x++;
		position.y++;
	}

	void setPos(int x, int y)
	{
		position.x = x;
		position.y = y;
	}
};