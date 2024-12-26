#pragma once
#include "Components.h"
#include "../Vector2D.h"

class TransformComponent : public Component
{
public:

	Vector2D position;
	Vector2D velocity;



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
		position.x;
		position.y;
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