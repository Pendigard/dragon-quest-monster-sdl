#include "Player.h"

Player::Player()
{
    gold = 0;
    mainTeam = std::vector<Monster>();
    benchTeam = std::vector<Monster>();
    storageMonsters = std::vector<Monster>();
}

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

Monster Player::getMonster(const int index, unsigned int team)
{
    assert(index >= 0 && index < 3);
    if (team == 1)
    {
        return mainTeam[index];
    }
    else if (team == 2)
    {
        return benchTeam[index];
    }
    else if (team == 3)
    {
        return storageMonsters[index];
    }
    return mainTeam[index];
}

void Player::addGold(const unsigned int g)
{
    gold += g;
}

void Player::removeGold(const unsigned int g)
{
    gold = std::max((unsigned int)0,gold-g);
}

unsigned int Player::getGold() const
{
    return gold;
}


void swapMonsters(const int index1, const int index2, std::vector<Monster> * team1, std::vector<Monster> * team2)
{
    if (index2 >= team2->size()) {
        team2->push_back((*team1)[index1]);
        team1->erase((*team1).begin() + index1);
    }
    else {
        std::swap((*team1)[index1], (*team2)[index2]);
    }
}

void Player::healAllMonsters()
{
    for (size_t i = 0; i < mainTeam.size(); i++)
    {
        mainTeam[i].fullHeal();
    }
    for (size_t i = 0; i < benchTeam.size(); i++)
    {
        benchTeam[i].fullHeal();
    }
    for (size_t i = 0; i < storageMonsters.size(); i++)
    {
        storageMonsters[i].fullHeal();
    }
}

void Player::healStorageMonsters()
{
    for (size_t i = 0; i < storageMonsters.size(); i++)
    {
        storageMonsters[i].fullHeal();
    }
}
