
#include "Hitbox.h"

Hitbox::Hitbox()
{
    this->x = 0;
    this->y = 0;
    this->w = 0;
    this->h = 0;
}

Hitbox::Hitbox(int x, int y, int w, int h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

int Hitbox::getX() const
{
    return this->x;
}

int Hitbox::getY() const
{
    return this->y;
}

int Hitbox::getW() const
{
    return this->w;
}

int Hitbox::getH() const
{
    return this->h;
}

void Hitbox::setX(int x)
{
    this->x = x;
}

void Hitbox::setY(int y)
{
    this->y = y;
}

void Hitbox::setW(int w)
{
    this->w = w;
}

void Hitbox::setH(int h)
{
    this->h = h;
}

bool Hitbox::isCollide(Hitbox h)
{
    if (this->x + this->w < h.getX() || this->x > h.getX() + h.getW())
        return false;
    if (this->y + this->h < h.getY() || this->y > h.getY() + h.getH())
        return false;
    return true;
}

