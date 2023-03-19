#ifndef _PLAYER
#define _PLAYER

#include <unordered_map>
#include <string>
#include <vector>
#include "Monster.h"


class Player
{
private:
    unsigned int gold;
public:
    std::vector<Monster> mainTeam;
    std::vector<Monster> benchTeam;
    std::vector<Monster> storageMonsters;

    void addMonster(const Monster& m,unsigned int team);

    void removeMonster(const int index,unsigned int team);

    void addGold(const unsigned int g);

    void removeGold(const unsigned int g);

    unsigned int getGold() const;

};

#endif