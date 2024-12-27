#pragma once

#include "../Game.h"
#include "ECS.h"
#include "Components.h"
#include "BallMechanic.h"


class Hole : public Component
{
public:

	TransformComponent* transform; 
	TransformComponent* ballpos;
	Entity* newPlayer;
	void setPlayer(Entity* player) {
		newPlayer = player; // добавление сущности м€ча
	}
	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		ballpos = &newPlayer->getComponent<TransformComponent>(); // (???) после удалени€ код запускаетс€
	}
};