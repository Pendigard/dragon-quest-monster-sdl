#ifndef SYNTHESIS_H
#define SYNTHESIS_H

#include "Monster.h"

std::vector<std::string> makeSynthesisPreview(Monster parent1, Monster parent2, Database &db);

bool isRecipe(Monster parent1, Monster parent2, Database &db);

std::string getMonsterFromRecipe(Monster parent1, Monster parent2, Database &db);

std::string getSynthesisRank(Monster parent1, Monster parent2);

std::string getChildMonster(Monster parent1,std::string rank);

std::unordered_map<std::string, unsigned int> getSynthesisSkills(Monster parent1, Monster parent2, std::string child, Database &db);

#endif