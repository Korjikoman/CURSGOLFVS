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
				setVelocity((mouseX - getInitialMousePos().x) / -60, (mouseY - getInitialMousePos().y) / -60);
				if (transform->velocity.x > 5)
				{
					transform->velocity.x = 5;
				}

				if (transform->velocity.y > 5)
				{
					transform->velocity.y = 5;
				}

				if (transform->velocity.x < -5)
				{
					transform->velocity.x = -5;
				}

				if (transform->velocity.y < -5)
				{
					transform->velocity.y = -5;
				}

				// ������� ������ �������� ����� ������� �������� (v0)
				transform->speed = SDL_sqrt(SDL_pow(abs(getVelocity().x), 2) + SDL_pow(abs(getVelocity().y), 2));
				
				// ��������� ����� ��� ������ �������������� ���������
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
	void draw() override
	{
		int mouseX = 0;
		int mouseY = 0;
		if (SDL_GetMouseState(&mouseX, &mouseY))
		{
			// ���������� ����� �����
			float dx = -1*(mouseX - getInitialMousePos().x);
			float dy = -1*(mouseY - getInitialMousePos().y);

			// ������������ �������
			float length = SDL_sqrt(dx * dx + dy * dy);
			if (length > 0)
			{
				dx /= length;
				dy /= length;
			}
			
			// ����� �����
			float lineLength = 100.0f;

			// ��������� ����������� �������
			float arrowLength = 15.0f; // ����� ����������� �������
			float arrowWidth = 10.0f;  // ������ ����������� �������

			// ������ ����
			float ballRadius = 16.0f; // �������� ������� ���� (32 / 2)

			// ������ ����� � ���������� ����
			float startX = transform->position.x + ballRadius + dx * ballRadius;
			float startY = transform->position.y + ballRadius + dy * ballRadius;

			// ����� �������� ����� (�������� ��������� �������)
			float lineEndX = startX + dx * (lineLength - arrowLength);
			float lineEndY = startY + dy * (lineLength - arrowLength);

			// ������ �������
			float arrowTipX = startX + dx * lineLength;
			float arrowTipY = startY + dy * lineLength;

			// ����� ����� ��������� ����������� �������
			float leftBaseX = lineEndX - arrowWidth / 2 * dy;
			float leftBaseY = lineEndY + arrowWidth / 2 * dx;

			// ������ ����� ��������� ����������� �������
			float rightBaseX = lineEndX + arrowWidth / 2 * dy;
			float rightBaseY = lineEndY - arrowWidth / 2 * dx;

			// ���������� ������ ����
			SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255);

			// ������ �������� �����
			SDL_RenderDrawLine(Game::renderer, static_cast<int>(startX), static_cast<int>(startY),
				static_cast<int>(lineEndX), static_cast<int>(lineEndY));

			// ������ ����������� (���������� �������)
			SDL_RenderDrawLine(Game::renderer, static_cast<int>(arrowTipX), static_cast<int>(arrowTipY),
				static_cast<int>(leftBaseX), static_cast<int>(leftBaseY));
			SDL_RenderDrawLine(Game::renderer, static_cast<int>(arrowTipX), static_cast<int>(arrowTipY),
				static_cast<int>(rightBaseX), static_cast<int>(rightBaseY));
			SDL_RenderDrawLine(Game::renderer, static_cast<int>(leftBaseX), static_cast<int>(leftBaseY),
				static_cast<int>(rightBaseX), static_cast<int>(rightBaseY));
		}
	}
};


