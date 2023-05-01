#include "Entity.h"

Entity::Entity()
{
    this->x = 0;
    this->y = 0;
    this->h = Hitbox();
    this->speed = 0;
    this->d = MOVE_DOWN;
    this->spritesID = std::vector<std::string>();
    this->f.frame = 1;
    this->timeLastMove = 0;
    this->timeLastFrame = 0;
    this->ai = AI_NONE;
    this->type = NPC;
    this->name = "NPC";
    moves.resize(4);
    collisions.resize(4);
}

Entity::Entity(int x, int y, int speed, std::vector<std::string> s, int w, int h, int xHitbox, int yHitbox)
{
    this->x = x;
    this->y = y;
    this->h = Hitbox(x, y, w, h, xHitbox, yHitbox);
    this->speed = speed;
    this->d = MOVE_DOWN;
    this->spritesID = s;
    this->f.frame = 1;
    this->timeLastMove = 0;
    this->timeLastFrame = 0;
    this->ai = AI_NONE;
    this->type = PLAYER;
    this->name = "Player";
    moves.resize(4);
    collisions.resize(4);
}

Entity::Entity(std::string monsterName, int x, int y, Database &db, aiType ai, Direction d)
{
    this->x = x;
    this->y = y;
    rapidjson::Value &monsterStat = db.getMonsterStatType(monsterName);
    unsigned int width = 32;
    unsigned int height = 32;
    unsigned int offsetX = 0;
    unsigned int offsetY = 0;
    if (monsterStat.HasMember("width"))
    {
        width = monsterStat["width"].GetInt();
    }
    if (monsterStat.HasMember("height"))
    {
        height = monsterStat["height"].GetInt();
    }
    if (monsterStat.HasMember("offsetX"))
    {
        offsetX = monsterStat["offsetX"].GetInt();
    }
    if (monsterStat.HasMember("offsetY"))
    {
        offsetY = monsterStat["offsetY"].GetInt();
    }
    this->h = Hitbox(x, y, width, height, offsetX, offsetY);
    this->speed = 1;
    this->d = MOVE_DOWN;
    this->name = monsterName;
    this->spritesID.push_back(monsterName + "_front");
    this->spritesID.push_back(monsterName + "_back");
    this->spritesID.push_back(monsterName + "_left");
    this->spritesID.push_back(monsterName + "_right");
    this->f.frame = 1;
    this->timeLastMove = 0;
    this->timeLastFrame = 0;
    moves.resize(4);
    collisions.resize(4);
    this->ai = ai;
    this->type = MONSTER;
}

Entity::Entity(std::string npcName, int x, int y, int w, int h, aiType ai, int offsetX, int offsetY, Direction d)
{
    this->x = x;
    this->y = y;
    this->h = Hitbox(x, y, w, h, offsetX, offsetY);
    this->speed = 1;
    this->d = d;
    this->name = npcName;
    if (npcName != "")
    {
        this->spritesID.push_back(npcName + "_front");
        this->spritesID.push_back(npcName + "_back");
        this->spritesID.push_back(npcName + "_left");
        this->spritesID.push_back(npcName + "_right");
    }
    this->f.frame = 1;
    this->timeLastMove = 0;
    this->timeLastFrame = 0;
    moves.resize(4);
    collisions.resize(4);
    this->ai = ai;
    this->type = NPC;
    destX = x;
    destY = y;
}

Entity &Entity::operator=(const Entity &e)
{
    this->x = e.x;
    this->y = e.y;
    this->h = e.h;
    this->speed = e.speed;
    this->spritesID = e.spritesID;
    this->d = e.d;
    this->f = e.f;
    this->moving = e.moving;
    this->moves = e.moves;
    this->collisions = e.collisions;
    this->ai = e.ai;
    this->type = e.type;
    this->timeLastMove = e.timeLastMove;
    this->timeLastFrame = e.timeLastFrame;
    this->name = e.name;
    this->script = e.script;
    this->destX = e.destX;
    this->destY = e.destY;
    return *this;
}

Entity::Entity(const Entity &e)
{
    this->x = e.x;
    this->y = e.y;
    this->h = e.h;
    this->speed = e.speed;
    this->spritesID = e.spritesID;
    this->d = e.d;
    this->f.frame = e.f.frame;
    this->moving = e.moving;
    this->moves = e.moves;
    this->collisions = e.collisions;
    this->ai = e.ai;
    this->type = e.type;
    this->timeLastMove = e.timeLastMove;
    this->timeLastFrame = e.timeLastFrame;
    this->name = e.name;
    this->script = e.script;
    this->destX = e.destX;
    this->destY = e.destY;
}

Entity::~Entity()
{
}

int Entity::getX() const
{
    return this->x;
}

int Entity::getY() const
{
    return this->y;
}

Hitbox &Entity::getHitbox()
{
    return h;
}

int Entity::getSpeed() const
{
    return this->speed;
}

void Entity::setSpeed(int s)
{
    this->speed = s;
}

void Entity::move(int x, int y)
{
    this->x = x;
    this->y = y;
    this->h.setX(x);
    this->h.setY(y);
}

void Entity::moveUp()
{
    d = MOVE_UP;
    if (!collisions[Top])
    {
        this->y -= this->speed;
        this->h.setY(this->y);
        moving = true;
    }
}

void Entity::moveDown()
{
    d = MOVE_DOWN;
    if (!collisions[Bottom])
    {
        this->y += this->speed;
        this->h.setY(this->y);
        moving = true;
    }
}

void Entity::moveLeft()
{
    d = MOVE_LEFT;
    if (!collisions[Left])
    {
        this->x -= this->speed;
        this->h.setX(this->x);
        moving = true;
    }
}

void Entity::moveRight()
{
    d = MOVE_RIGHT;
    if (!collisions[Right])
    {
        this->x += this->speed;
        this->h.setX(this->x);
        moving = true;
    }
}

std::string Entity::getSpriteID() const
{
    return this->spritesID[this->d];
}

void Entity::updateFrame()
{
    if (!moving)
    {
        f.frame = 1;
        return;
    }
    if (f.ascending)
    {
        f.frame++;
        if (f.frame == 2)
        {
            f.frame = 2;
            f.ascending = false;
        }
    }
    else
    {
        f.frame--;
        if (f.frame == 0)
        {
            f.frame = 0;
            f.ascending = true;
        }
    }
}

int Entity::getFrame() const
{
    return f.frame;
}

void Entity::update()
{
    moving = false;
    if (moves[MOVE_UP])
        moveUp();
    else if (moves[MOVE_DOWN])
        moveDown();
    else if (moves[MOVE_LEFT])
        moveLeft();
    else if (moves[MOVE_RIGHT])
        moveRight();
    if (timeLastFrame + 125 < SDL_GetTicks())
    {
        timeLastFrame = SDL_GetTicks();
        updateFrame();
    }
}

void Entity::clearCollision()
{
    collisions[Top] = false;
    collisions[Bottom] = false;
    collisions[Left] = false;
    collisions[Right] = false;
}

void Entity::setCollision(CollisionSide collision)
{
    if (collision == None)
        return;
    collisions[collision] = true;
}

void Entity::clearMoves()
{
    moves[MOVE_UP] = false;
    moves[MOVE_DOWN] = false;
    moves[MOVE_LEFT] = false;
    moves[MOVE_RIGHT] = false;
}

void Entity::updateAi()
{
    int random = rand() % 8;
    switch (ai)
    {
    case AI_RANDOM:
        if (timeLastMove + 500 < SDL_GetTicks())
        {
            timeLastMove = SDL_GetTicks();
            clearMoves();
            if (random < 4)
                moves[random] = true;
        }
        break;
    case AI_LEFT_RIGHT:
        if (atDestination())
        {
            if (moving)
                timeLastMove = SDL_GetTicks();
            clearMoves();
            if (timeLastMove + 2000 < SDL_GetTicks())
            {
                if (d == MOVE_LEFT)
                    changeDestination(x + 100, y);
                else
                    changeDestination(x - 100, y);
            }
        }
        if (!atDestination())
        {
            moveToDestination();
        }
        break;
    default:
        break;
    }
    update();
}

std::string Entity::getName() const
{
    return name;
}

entityTypes Entity::getType() const
{
    return type;
}

Direction Entity::getDirection() const
{
    return d;
}

void Entity::setScript(std::string scriptName)
{
    script = scriptName;
}

std::string Entity::getScript() const
{
    return script;
}

void Entity::collision(Hitbox &h)
{
    CollisionSide collision = getHitbox().getCollision(h);
    setCollision(collision);
    if (collision != None)
    {
        int x = getX();
        int y = getY();
        h.getHitboxBorder(x, y, collision, getHitbox());
        move(x, y);
    }
}

bool Entity::collided(Entity &e)
{
    return getHitbox().isColliding(e.getHitbox());
}

bool Entity::inFrontOf(Entity &e) const
{
    if (e.getDirection() == MOVE_UP && d == MOVE_DOWN)
        return true;
    if (e.getDirection() == MOVE_DOWN && d == MOVE_UP)
        return true;
    if (e.getDirection() == MOVE_LEFT && d == MOVE_RIGHT)
        return true;
    if (e.getDirection() == MOVE_RIGHT && d == MOVE_LEFT)
        return true;
    return false;
}

void Entity::changeDestination(int x, int y)
{
    this->destX = x;
    this->destY = y;
}

void Entity::moveToDestination()
{
    if (x < destX)
        moves[MOVE_RIGHT] = true;
    else if (x > destX)
        moves[MOVE_LEFT] = true;
    else if (y < destY)
        moves[MOVE_DOWN] = true;
    else if (y > destY)
        moves[MOVE_UP] = true;
}

bool Entity::atDestination() const
{
    return x == destX && y == destY;
}

aiType Entity::getAi() const
{
    return ai;
}