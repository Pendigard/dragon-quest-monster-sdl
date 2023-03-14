#ifndef SYNTHESIS_H
#define SYNTHESIS_H

#include "Monster.h"

std::vector<std::string> makeSynthesis(Monster parent1, Monster parent2);

std::string getSynthesisRank(Monster parent1, Monster parent2);

std::string getChildMonster(Monster parent1,std::string rank);

#endif