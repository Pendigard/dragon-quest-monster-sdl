#include "Fight.h"

Action createAction(std::string i, std::string s, std::vector<std::string> targets)
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

std::vector<std::string> Fight::getTargetTactic(Monster caster, std::string spell, bool isTeam1)
{
    rapidjson::Value &spellInfo = spellBase[spell.c_str()];
    std::vector<std::string> targetTeam;
    if (spellInfo["offensive"].GetBool())
    {
        if (spellInfo["multiTarget"].GetBool())
        {
            if (isTeam1)
            {
                for (int i = 0; i < team2.size(); i++)
                {
                    targetTeam.push_back(team2[i].getId());
                }
            }
            else
            {
                for (int i = 0; i < team1.size(); i++)
                {
                    targetTeam.push_back(team1[i].getId());
                }
            }
        }
        else
        {
            if (isTeam1)
            {
                targetTeam.push_back(team2[rand() % team2.size()].getId());
            }
            else
            {
                targetTeam.push_back(team1[rand() % team1.size()].getId());
            }
        }
    }
    else
    {
        if (spellInfo["multiTarget"].GetBool())
        {
            if (isTeam1)
            {
                for (int i = 0; i < team1.size(); i++)
                {
                    targetTeam.push_back(team1[i].getId());
                }
            }
            else
            {
                for (int i = 0; i < team2.size(); i++)
                {
                    targetTeam.push_back(team2[i].getId());
                }
            }
        }
        else
        {
            if (isTeam1)
            {
                targetTeam.push_back(team1[rand() % team1.size()].getId());
            }
            else
            {
                targetTeam.push_back(team2[rand() % team2.size()].getId());
            }
        }
    }
    return targetTeam;
}

void Fight::spellTacticChoice(Monster &caster, bool isTeam1)
{
    // Temporaire
    std::vector<std::string> spells = caster.getSpells();
    unsigned int randomSpell = rand() % spells.size();
    std::string spell = caster.getSpells()[randomSpell];
    std::vector<std::string> targetTeam = getTargetTactic(caster, spell, isTeam1);
    actions.push_back(createAction(caster.getId(), spell, targetTeam));
}

Monster& Fight::getMonsterById(std::string id)
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
    for (int i = 0; i < actions.size(); i++)
    {
        if (actions[i].spell == "Defense")
        {
            actionsOrdered.push(actions[i]);
            actions.erase(actions.begin() + i);
        }
    }
    int maxIndex;
    while (actions.size() > 0)
    {
        maxIndex = 0;
        for (int i = 0; i < actions.size(); i++)
        {
            if (getMonsterById(actions[maxIndex].idCaster).getAgility() < getMonsterById(actions[i].idCaster).getAgility())
                maxIndex = i;
        }
        actionsOrdered.push(actions[maxIndex]);
        actions.erase(actions.begin() + maxIndex);
    }
}

void Fight::giveActions(std::vector<Action> action)
{
    for (int i = 0; i < action.size(); i++)
    {
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
        if (std::count(alreadyAction.begin(), alreadyAction.end(), team1[i].getId()) == 0)
            spellTacticChoice(team1[i], true);
    }
    for (int i = 0; i < team2.size(); i++)
    {
        if (std::count(alreadyAction.begin(), alreadyAction.end(), team2[i].getId()) == 0)
            spellTacticChoice(team2[i], false);
    }
    createStackAgility();
}

std::queue<spellImpact> Fight::simulateAction()
{
    assert(actionsOrdered.size() > 0);
    Action a = actionsOrdered.front();
    actionsOrdered.pop();
    Monster caster = getMonsterById(a.idCaster);
    if (caster.hp == 0)
        return std::queue<spellImpact>();
    rapidjson::Value &spellInfo = spellBase[a.spell.c_str()];
    spellImpact impact;
    std::queue<spellImpact> queueImpact;
    bool multipleTarget = a.idTargets.size() > 1;
    if (a.spell == "Attaque")
        impact.message = caster.getName() + " attaque " + getMonsterById(a.idTargets[0]).getName();
    else if (a.spell == "Defense")
        impact.message = caster.getName() + " adopte une posture défensive";
    else
    {
        impact.message = caster.getName() + " lance " + a.spell;
        if (multipleTarget)
            impact.message += " sur l'équipe";
        else {
            if (getMonsterById(a.idTargets[0]).getName() == caster.getName())
                impact.message += " sur lui-même";
            else
            impact.message += " sur " + getMonsterById(a.idTargets[0]).getName();
        }   
            
    }
    queueImpact.push(impact);
    std::string type = spellInfo["type"].GetString();
    if (spellInfo["offensive"].GetBool())
    {
        if (spellInfo["dealDamage"].GetBool())
        {
            float dmg;
            for (int i = 0; i < a.idTargets.size(); i++)
            {
                impact.function = damage;
                if ("physical" == type)
                {
                    dmg = (caster.getStat("atk") / 2 - getMonsterById(a.idTargets[i]).getStat("def") / 4) * spellInfo["damage"].GetFloat();
                }
                else
                {
                    dmg = (caster.getStat("wis") / 2 - getMonsterById(a.idTargets[i]).getStat("def") / 4) * spellInfo["damage"].GetFloat();
                }
                if (dmg < 0)
                    dmg = 0;
                impact.targetId.push_back(a.idTargets[i]);
                impact.argumentFloat.push_back(dmg);
            }
            if (multipleTarget)
                impact.message = "Les monstres adverses subissent " + std::to_string((int)dmg) + " points de dégâts";
            else if (dmg == 0)
                impact.message = caster.getName() + " rate son attaque";
            else
                impact.message = getMonsterById(a.idTargets[0]).getName() + " subit " + std::to_string((int)dmg) + " points de dégâts";
            queueImpact.push(impact);
        }
    }
    else
    {
        if ("heal" == type)
        {
            assert(spellInfo.HasMember("heal"));
            impact.function = heal;
            float heal = spellInfo["heal"].GetFloat();
            for (int i = 0; i < a.idTargets.size(); i++)
            {
                impact.targetId.push_back(a.idTargets[i]);
                impact.argumentFloat.push_back(heal);
            }
            if (multipleTarget)
                impact.message = "Les monstres alliés sont soignés de " + std::to_string((int)heal) + " points de vie";
            else
                impact.message = getMonsterById(a.idTargets[0]).getName() + " se soigne de " + std::to_string((int)heal) + " points de vie";
            queueImpact.push(impact);
        }
    }
    return queueImpact;
}

void Fight::updateMonster(spellImpact impact)
{
    for (int i = 0; i < impact.targetId.size(); i++)
    {
        Monster &m = getMonsterById(impact.targetId[i]);
        switch (impact.function)
        {
        case damage:
            m.damage(impact.argumentFloat[i]);
            break;
        case heal:
            m.heal(impact.argumentFloat[i]);
            break;
        }
    }
}
