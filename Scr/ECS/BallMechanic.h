#include "../Game.h"
#include "ECS.h"
#include "Components.h"

class Ball : public Component
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


	// ������ ��������
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
				// ������� ��������� ������������ �������� �����
				SDL_GetMouseState(&mouseX, &mouseY);
				// ����������� ��� ���������� initialMousePos.x � initialMousePos.y 
				setInitialMousePos(mouseX, mouseY);
				std::cout << mouseX << "   ;    " << mouseY << std::endl;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (Game::event.button.button == SDL_BUTTON_LEFT)
			{


				SDL_GetMouseState(&mouseX, &mouseY);
				
				// ������ ������ ��������
				setVelocity((mouseX - getInitialMousePos().x) / -150, (mouseY - getInitialMousePos().y) / -150);
				// ������� ������ �������� ����� ������� �������� (v0)
				transform->speed = SDL_sqrt(SDL_pow(abs(getVelocity().x), 2) + SDL_pow(abs(getVelocity().y), 2));
				// a = (0-v0)/t = -v0/t
				transform->acceleration = transform->speed / 10;
				std::cout << transform->speed << std::endl;
			}
			break;
		}

	}
};


