#include "ECS.h"

Manager::Manager()
{
}


void Entity::addGroup(Group mGroup)
{
	groupBitset[mGroup] = true;
	manager.AddToGroup(this, mGroup);
}

