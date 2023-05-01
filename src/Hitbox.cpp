
#include "Hitbox.h"

Hitbox::Hitbox()
{
    this->x = 0;
    this->y = 0;
    this->offsetX = 0;
    this->offsetY = 0;
    this->w = 0;
    this->h = 0;
}

Hitbox::Hitbox(int x, int y, int w, int h, int offsetX, int offsetY, HitboxType type)
{
    this->x = x;
    this->y = y;
    this->offsetX = offsetX;
    this->offsetY = offsetY;
    this->w = w;
    this->h = h;
    this->type = type;
}

Hitbox &Hitbox::operator=(const Hitbox &h)
{
    this->x = h.getX();
    this->y = h.getY();
    this->offsetX = h.offsetX;
    this->offsetY = h.offsetY;
    this->w = h.getWidth();
    this->h = h.getHeight();
    return *this;
}

int Hitbox::getX() const
{
    return this->x;
}

int Hitbox::getY() const
{
    return this->y;
}

int Hitbox::getOffsetX() const
{
    return this->offsetX;
}

int Hitbox::getOffsetY() const
{
    return this->offsetY;
}

int Hitbox::getRealX() const
{
    return this->x + this->offsetX;
}

int Hitbox::getRealY() const
{
    return this->y + this->offsetY;
}

int Hitbox::getWidth() const
{
    return this->w;
}

int Hitbox::getHeight() const
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

CollisionSide Hitbox::getCollision(Hitbox h) const
{
    if (this->getRealX() + this->w < h.getRealX())
        return None;
    else if (this->getRealX() > h.getRealX() + h.getWidth())
        return None;
    else if (this->getRealY() + this->h < h.getRealY())
        return None;
    else if (this->getRealY() > h.getRealY() + h.getHeight())
        return None;
    
    std::vector<int> distances;
    distances.resize(4);

    distances[Top] = std::abs((this->getRealY()) - (h.getRealY() + h.getHeight()));
    distances[Bottom] = std::abs((this->getRealY() + this->h) - (h.getRealY()));
    distances[Left] = std::abs((this->getRealX()) - (h.getRealX() + h.getWidth()));
    distances[Right] = std::abs((this->getRealX() + this->w) - (h.getRealX()));
    
    int min = distances[Top];
    CollisionSide side = Top;
    for (int i = 1; i < 4; i++)
    {
        if (distances[i] < min)
        {
            min = distances[i];
            side = (CollisionSide)i;
        }
    }
    return side;
}

void Hitbox::getHitboxBorder(int &x, int &y, CollisionSide side, Hitbox h) const
{
    switch (side)
    {
    case Bottom:
        y = this->getRealY() - h.getHeight() - h.getOffsetY();
        break;
    case Top:
        y = this->getRealY() + this->h - h.getOffsetY();
        break;
    case Right:
        x = this->getRealX() - h.getWidth() - h.getOffsetX();
        break;
    case Left:
        x = this->getRealX() + this->w - h.getOffsetX();
        break;
    default:
        break;
    }
}

bool Hitbox::isColliding(Hitbox h) const
{
    return this->getCollision(h) != None;
}