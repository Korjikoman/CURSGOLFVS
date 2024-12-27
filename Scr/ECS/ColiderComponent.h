#pragma once

#include <string>
#include "SDL.h"
#include "ECS.h"
#include "Components.h"
class ColliderComponent : public Component
{
	SDL_Rect collider;
	std::string tag;

	TransformComponent* transform;

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{

			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override {
		collider.x = transform->position.x;

	}
};
