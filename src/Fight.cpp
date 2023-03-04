#include "GameTxt.h"

struct Action
{
    Monster caster;
    std::vector<Monster> targets;        
    bool isEnemy;
    std::string spell;

};

Action newAction(Monster c, std::vector<Monster> t, bool isE, std::string s) {
    Action a;
    a.caster = c;
    a.targets = t;
    a.isEnemy = isE;
    a.spell = s;
    return a;
}