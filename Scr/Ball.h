#include "Game.h"
#include "ECS/ECS.h"
#include "ECS/Components.h"

class Ball : public Component
{
public:

	TransformComponent* transform;
	Vector2D target;
	Vector2D launchedVelocity;
	Vector2D initialMousePos;
	Vector2D currentMousePos;
	float velocity1D;
	float launchedVelocity1D;
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

	void setLaunchedVelocity(float x, float y)
	{
		launchedVelocity.x = x;
		launchedVelocity.y = y;
	}

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
		if (mousePressed) //не работает, надо брать переменные из класса game?
		{
			SDL_GetMouseState(&mouseX, &mouseY);
			setInitialMousePos(mouseX, mouseY);
		}
		if (mouseDown) {
			
			SDL_GetMouseState(&mouseX, &mouseY);
			setVelocity((mouseX - getInitialMousePos().x) / -150, (mouseY - getInitialMousePos().y) / -150);
			setLaunchedVelocity((mouseX - getInitialMousePos().x) / -150, (mouseY - getInitialMousePos().y) / -150);
			velocity1D = SDL_sqrt(SDL_pow(abs(getVelocity().x), 2) + SDL_pow(abs(getVelocity().y), 2));
			launchedVelocity1D = velocity1D;
		}
		
	}
};