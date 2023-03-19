#include "Player.h"

void Player::addMonster(const Monster &m, unsigned int team)
{
    if (team == 1)
    {
        if (mainTeam.size() < 3)
        {
            mainTeam.push_back(m);
        }
    }
    else if (team == 2)
    {
        if (benchTeam.size() < 3)
        {
            benchTeam.push_back(m);
        }
    }
    else if (team == 3)
    {
        storageMonsters.push_back(m);
    }
}

void Player::removeMonster(const int index, unsigned int team)
{
    assert(index >= 0 && index < 3);
    if (team == 1)
    {
        if (mainTeam.size() > 1)
        {
            mainTeam.erase(mainTeam.begin() + index);
        }
    }
    else if (team == 2)
    {
        if (benchTeam.size() > 1)
        {
            benchTeam.erase(benchTeam.begin() + index);
        }
    }
    else if (team == 3)
    {
        storageMonsters.erase(storageMonsters.begin() + index);
    }

}

void Player::addGold(const unsigned int g)
{
    gold += g;
}

void Player::removeGold(const unsigned int g)
{
    gold -= std::min(g,gold);
}

unsigned int Player::getGold() const
{
    return gold;
}