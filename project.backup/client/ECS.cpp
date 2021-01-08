#include "ECS.h"

void Entity::addGroup(Group mGroup)
{
	groupBitset[mGroup] = true;
	Manager.AddToGroup(this, mGroup);
}
