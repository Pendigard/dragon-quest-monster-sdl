#include "Fight.h"

Action createAction(std::string i, std::string s, std::vector<Monster> targets)
{
    Action a;
    a.idCaster = i;
    a.idTargets = targets;
    a.spell = s;
    return a;
}

Fight::Fight(std::vector<Monster> t1, std::vector<Monster> t2)
{
    nbTurn = 0;
    team1 = t1;
    team2 = t2;
    loadJson("data/database/spell.json", spellBase);
}

bool Fight::isTeam1(Monster m) const
{
    for (int i = 0; i < team1.size(); i++)
    {
        if (team1[i] == m)
        {
            return true;
        }
    }
    return false;
}


std::vector<Monster> Fight::getTargetTactic(Monster caster, std::string spell, bool isTeam1)
{
    rapidjson::Value &spellInfo = spellBase[spell.c_str()];
    std::vector<Monster> targetTeam;
    if (spellInfo["offensive"].GetBool())
    {
        if (spellInfo["multiTarget"].GetBool())
        {
            if (isTeam1) return team2;
            else return team1;
        }
        else
        {
            if (isTeam1)
            {
                targetTeam.push_back(team2[rand() % team2.size()]);
                return targetTeam;
            }
            else
            {
                targetTeam.push_back(team1[rand() % team1.size()]);
                return targetTeam;
            }
        }
    }
    else
    {
        if (spellInfo["multiTarget"].GetBool())
        {
            if (isTeam1) return team1;
            else return team2;
        }
        else
        {
            if (isTeam1)
            {
                targetTeam.push_back(team1[rand() % team1.size()]);
                return targetTeam;
            }
            else
            {
                targetTeam.push_back(team2[rand() % team2.size()]);
                return targetTeam;
            }
        }
    }
}

Action Fight::spellTacticChoice(Monster &caster, bool isTeam1)
{
    // Temporaire
    std::vector<std::string> spells = caster.getSpells();
    unsigned int randomSpell = rand() % spells.size();
    std::string spell = caster.getSpells()[randomSpell];
    std::vector<Monster> targetTeam = getTargetTactic(caster, spell, isTeam1);
    return createAction(caster.getId(), spell, targetTeam);
   
}

Monster Fight::getMonsterById(std::string id)
{
    for (int i = 0; i < team1.size(); i++)
    {
        if (team1[i].getId() == id)
        {
            return team1[i];
        }
    }
    for (int i = 0; i < team2.size(); i++)
    {
        if (team2[i].getId() == id)
        {
            return team2[i];
        }
    }
    return team1[0]; // A changer
}

void Fight::createStackAgility()
{
    actionsOrdered.empty();
    int minIndex;
    while (actions.size() > 0)
    {
        minIndex = 0;
        for (int i = 0; i < actions.size(); i++)
        {
            if (getMonsterById(actions[minIndex].idCaster).getAgility() > getMonsterById(actions[i].idCaster).getAgility())
                minIndex = i;
        }
        actionsOrdered.push(actions[minIndex]);
        actions.erase(actions.begin() + minIndex);
    }
}


void Fight::giveActions(std::vector<Action> action)
{
    for (int i = 0; i < action.size(); i++) {
        actions.push_back(action[i]);
    }
}

void Fight::initTurn()
{
    std::vector<std::string> alreadyAction;
    for (int i = 0; i < actions.size(); i++)
    {
        alreadyAction.push_back(actions[i].idCaster);
    }
    for (int i = 0; i < team1.size(); i++)
    {
        if (std::count(alreadyAction.begin(), alreadyAction.end(),team1[i].getId()) == 0) spellTacticChoice(team1[i], true);
    }
    for (int i = 0; i < team2.size(); i++)
    {
        if (std::count(alreadyAction.begin(), alreadyAction.end(),team2[i].getId()) == 0) spellTacticChoice(team2[i], false);
    }
    createStackAgility();

}

