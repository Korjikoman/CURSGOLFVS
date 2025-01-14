#pragma once
#include "../Game.h"
#include "ECS.h"
#include "Components.h"
#include <SDL_ttf.h>
#include <sstream>
#include "SDL_mixer.h"
#include "SDL.h"

class Particle {
public:
	Vector2D position;
	Vector2D velocity;
	float lifetime; // ����� ����� ������� � ��������
	float size; // ������ �������

	Particle(float x, float y, float vx, float vy, float lifetime, float size)
		: position(x, y), velocity(vx, vy), lifetime(lifetime), size(size) {}

	void update(float deltaTime) {
		position.x += velocity.x * deltaTime;
		position.y += velocity.y * deltaTime;
		lifetime -= deltaTime;
	}

	bool isDead() const {
		return lifetime <= 0;
	}
};

class BallMechanic : public Component
{
public:

	TransformComponent* transform;
	Vector2D target;
	Vector2D launchedVelocity;
	Vector2D initialMousePos;
	Vector2D currentMousePos;
	Mix_Chunk* swingSound = nullptr;

	bool canMove = true;
	bool playedSwingFx = true;
	int index;
	int strokes = 0;
	int dirX = 1;
	int dirY = 1;
	float velocityX;
	float velocityY;
	float velocityLength;

	bool strokesLimitFlag = false;
	bool win = false;
	float friction = 0.001;
	bool isMoving = false;  // ���� ��� ��������, ��� ��� ���������

	float stopThreshold = 0.005f;
	float stopConst = 0.002f;

	std::vector<std::unique_ptr<Particle>> particles;

	std::string to_string(int value) {
		std::stringstream ss;
		ss << value;
		return ss.str();
	}

	template <typename T>
	T clamp(T value, T min, T max) {
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		swingSound = Mix_LoadWAV("assets/sfx/swing.mp3");
		if (!swingSound) {
			std::cerr << "Failed to load swing sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
		}
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
		Game::win = p_win;
	}

	void update() override
	{
		float deltaTime = 1.0f / 60.0f;

		// ���������, ��������� �� ���
		if (SDL_sqrt(transform->velocity.x * transform->velocity.x + transform->velocity.y * transform->velocity.y) > stopThreshold)
		{
			isMoving = true;
		}
		else
		{
			isMoving = false;
		}
		if (isMoving) {
			// ��������� ����� ������ ������ ����
			float speed = SDL_sqrt(transform->velocity.x * transform->velocity.x + transform->velocity.y * transform->velocity.y);
			float particleSize = clamp(8.0f - speed * 1.5f, 2.0f, 8.0f); // ������ ������� ����������� � ����������� ��������
			particles.emplace_back(std::make_unique<Particle>(
				transform->position.x + 13, // ����� ����
				transform->position.y + 13,
				-transform->velocity.x * 0.5f, // ������� ���������, ��� ���
				-transform->velocity.y * 0.5f,
				0.5f, // ����� ����� �������
				particleSize
			));
		}

		// ��������� �������
		for (auto& particle : particles) {
			particle->update(deltaTime);
		}

		// ������� ������� �������
		particles.erase(
			std::remove_if(particles.begin(), particles.end(), [](const std::unique_ptr<Particle>& p) { return p->isDead(); }),
			particles.end()
		);
		if (!isMoving)  // ��������� ����� ���� ������ ���� ��� �� ���������
		{
			int mouseX = 0;
			int mouseY = 0;

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
			case SDL_MOUSEMOTION:
				if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT))
				{
					// ��������� ��������� ���� � ������������ ��������
					SDL_GetMouseState(&mouseX, &mouseY);

					velocityX = (mouseX - getInitialMousePos().x) / -60.0f;
					velocityY = (mouseY - getInitialMousePos().y) / -60.0f;

					// ������������ �������� ��������
					velocityX = clamp(velocityX, -5.0f, 5.0f);
					velocityY = clamp(velocityY, -5.0f, 5.0f);
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (Game::event.button.button == SDL_BUTTON_LEFT)
				{
					SDL_GetMouseState(&mouseX, &mouseY);
					// ������ ������ ��������
					setVelocity((mouseX - getInitialMousePos().x) / -60, (mouseY - getInitialMousePos().y) / -60);

					float velocityX1 = transform->velocity.x;
					float velocityY1 = transform->velocity.y;
					float velocityMagnitude = sqrt(velocityX1 * velocityX1 + velocityY1 * velocityY1);

					// ������������� ������������ ��������
					const float maxVelocity = 5.0f; 
					if (velocityMagnitude > maxVelocity) {
						float normalizationFactor = maxVelocity / velocityMagnitude;
						transform->velocity.x *= normalizationFactor;
						transform->velocity.y *= normalizationFactor;
					}

					// ������� ������ �������� ����� ������� �������� (v0)
					transform->speed = SDL_sqrt(SDL_pow(abs(getVelocity().x), 2) + SDL_pow(abs(getVelocity().y), 2));

					// ��������� ����� ��� ������ �������������� ���������
					transform->acceleration.x = -transform->velocity.x * stopConst;
					transform->acceleration.y = -transform->velocity.y * stopConst;

					// ����������� ���������� ������
					strokes++;
					if (swingSound) {
						Mix_PlayChannel(-1, swingSound, 0);
					}
				}
				break;
			}
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
			
			velocityLength = SDL_sqrt(velocityX * velocityX + velocityY * velocityY);
			// ����� �����
			// ����������� � ������������ ����� �����
			float minLength = 16.0f;
			float maxLength = 80.0f;

			// ����� ����� ���������� � ����������� �� �������� ����
			float lineLength = minLength + (velocityLength * 20.0f); // ����������� ����� � ����������� �� ��������

			// ������������ ����� �����
			if (lineLength > maxLength) {
				lineLength = maxLength;
			}
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
		for (const auto& particle : particles) {
			SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255); // ����� ����
			SDL_Rect rect = { static_cast<int>(particle->position.x), static_cast<int>(particle->position.y), static_cast<int>(particle->size), static_cast<int>(particle->size) };
			SDL_RenderFillRect(Game::renderer, &rect);
		}
	}

	bool isWin() const
	{
		return Game::win; // ��������� ���������� ����������
	}
};


