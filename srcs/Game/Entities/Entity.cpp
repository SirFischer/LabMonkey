#include "Entity.hpp"

Entity::Entity(/* args */)
{
}

Entity::~Entity()
{
}

void				Entity::MoveLeft()
{
	mVelocity.x -= mSpeed;
}

void				Entity::MoveRight()
{
	mVelocity.x += mSpeed;
}