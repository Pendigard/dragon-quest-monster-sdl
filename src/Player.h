#ifndef _PLAYER
#define _PLAYER

#include <unordered_map>
#include <string>
#include <vector>
#include "Monster.h"


class Player
{
public:
    std::vector<Monster> mainTeam;
    std::vector<Monster> benchTeam;
    std::vector<Monster> storageMonsters;

};

#endif