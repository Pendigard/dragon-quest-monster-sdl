#include "Entity.h"

Entity::Entity(int x, int y, Hitbox h, int speed, std::vector<std::string> s)
{
    this->x = x;
    this->y = y;
    this->h = h;
    this->speed = speed;
    this->d = LOOK_DOWN;
    this->spritesID = s;
}


int Entity::getX() const
{
    return this->x;
}

int Entity::getY() const
{
    return this->y;
}

Hitbox Entity::getHitbox() const
{
    return this->h;
}

int Entity::getSpeed() const
{
    return this->speed;
}

void Entity::move(int x, int y)
{
    this->x = x;
    this->y = y;
}

void Entity::moveUp()
{
    this->y -= this->speed;
}

void Entity::moveDown()
{
    this->y += this->speed;
}

void Entity::moveLeft()
{
    this->x -= this->speed;
}

void Entity::moveRight()
{
    this->x += this->speed;
}
