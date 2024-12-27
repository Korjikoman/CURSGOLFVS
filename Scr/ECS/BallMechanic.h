#pragma once
#include "../Game.h"
#include "ECS.h"
#include "Components.h"

class BallMechanic : public Component
{
public:

	TransformComponent* transform;
	Vector2D target;
	Vector2D launchedVelocity;
	Vector2D initialMousePos;
	Vector2D currentMousePos;
	

	
	bool canMove = true;
	bool playedSwingFx = true;
	int index;
	int strokes = 0;
	int dirX = 1;
	int dirY = 1;
	bool win = false;
	float friction = 0.001;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
	}

	Vector2D& getVelocity()
	{
		return transform->velocity;
	}

	Vector2D& getInitialMousePos()
	{
		return initialMousePos;
	}

	void setVelocity(float x, float y)
	{
		transform->velocity.x = x;
		transform->velocity.y = y;
	}

	/*void setLaunchedVelocity(float x, float y)
	{
		launchedVelocity.x = x;
		launchedVelocity.y = y;
	}*/

	void setInitialMousePos(float x, float y)
	{
		initialMousePos.x = x;
		initialMousePos.y = y;
	}

	void setCurrentMousePos(float x, float y)
	{
		currentMousePos.x = x;
		currentMousePos.y = y;
	}


	// вектор скорости
	float getDistance(Vector2D a, Vector2D b) {
		return SDL_sqrt(SDL_pow(abs(a.x - b.x), 2) + SDL_pow(abs(a.y - b.y), 2));
	}

	void setWin(bool p_win)
	{
		win = p_win;
	}

	void update() override
	{
		int mouseX = 0;
		int mouseY = 0;
		float length = 0;

		switch (Game::event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			if (Game::event.button.button == SDL_BUTTON_LEFT)
			{
				// находим положение относительно рабочего стола
				SDL_GetMouseState(&mouseX, &mouseY);
				// присваиваем эти координаты initialMousePos.x и initialMousePos.y 
				setInitialMousePos(mouseX, mouseY);
				std::cout << mouseX << "   ;    " << mouseY << std::endl;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (Game::event.button.button == SDL_BUTTON_LEFT)
			{


				SDL_GetMouseState(&mouseX, &mouseY);
				
				// задаем вектор скорости
				setVelocity((mouseX - getInitialMousePos().x) / -60, (mouseY - getInitialMousePos().y) / -60);
				// находим модуль скорости через теорему пифагора (v0)
				transform->speed = SDL_sqrt(SDL_pow(abs(getVelocity().x), 2) + SDL_pow(abs(getVelocity().y), 2));
				
				// замедляем шарик при помощи отрицательного ускорения
				transform->acceleration.x = -transform->velocity.x * 0.002f;
				transform->acceleration.y = -transform->velocity.y * 0.002f;
			}
			break;
		}
	}
	Vector2D getPos()
	{
		return transform->position;
	}
};


