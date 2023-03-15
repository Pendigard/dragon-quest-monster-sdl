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

std::vector<std::string> Fight::getTeamAlive(std::vector<Monster> team) const
{
    std::vector<std::string> alive;
    for (int i = 0; i < team.size(); i++)
    {
        if (team[i].hp > 0)
        {
            alive.push_back(team[i].getInfos("id"));
        }
    }
    return alive;
}

std::vector<std::string> Fight::getTargetTactic(Monster caster, std::string spell)
{
    rapidjson::Value &spellInfo = spellBase[spell.c_str()];
    std::vector<std::string> targets;
    std::vector<std::string> targetTeam;
    if (isTeam1(caster))
    {
        if (spellInfo["offensive"].GetBool())
            targetTeam = getTeamAlive(team2);
        else
            targetTeam = getTeamAlive(team1);
    }
    else
    {
        if (spellInfo["offensive"].GetBool())
            targetTeam = getTeamAlive(team1);
        else
            targetTeam = getTeamAlive(team2);
    }
    if (spellInfo["multiTarget"].GetBool())
    {
        targets = targetTeam;
    }
    else if (spellInfo["self"].GetBool())
    {
        targets.push_back(caster.getInfos("id"));
    }
    else
    {
        targets.push_back(targetTeam[rand() % targetTeam.size()]);
    }
    return targets;
}

Action Fight::tacticSoin(Monster caster)
{
    Action a;
    std::vector<std::string> spells = caster.getSpells();
    std::vector<std::string> possibleSpells;
    possibleSpells.push_back("Attaque");
    for (int i = 0; i < spells.size(); i++)
    {
        rapidjson::Value &spellInfo = spellBase[spells[i].c_str()];
        if (spellInfo.HasMember("heal"))
            possibleSpells.push_back(spells[i]);
    }
    std::string spell = possibleSpells[getRand(0, possibleSpells.size())];
    a = createAction(caster.getInfos("id"), spell, getTargetTactic(caster, spell));
    return a;
}

Action Fight::tacticNoMana(Monster caster)
{
    Action a;
    std::vector<std::string> spells = caster.getSpells();
    std::vector<std::string> possibleSpells;
    for (int i = 0; i < spells.size(); i++)
    {
        rapidjson::Value &spellInfo = spellBase[spells[i].c_str()];
        if (spellInfo["manaCost"].GetInt() == 0)
            possibleSpells.push_back(spells[i]);
    }
    std::string spell = possibleSpells[getRand(0, possibleSpells.size())];
    a = createAction(caster.getInfos("id"), spell, getTargetTactic(caster, spell));
    return a;
}

Action Fight::tacticSagesse(Monster caster)
{
    Action a;
    std::vector<std::string> spells = caster.getSpells();
    std::vector<std::string> possibleSpells;
    for (int i = 0; i < spells.size(); i++)
    {
        rapidjson::Value &spellInfo = spellBase[spells[i].c_str()];
        if (spellInfo["manaCost"].GetInt() <= caster.mp)
            possibleSpells.push_back(spells[i]);
    }
    std::string spell = possibleSpells[getRand(0, possibleSpells.size())];
    a = createAction(caster.getInfos("id"), spell, getTargetTactic(caster, spell));
    return a;
}

Action Fight::tacticSansPitie(Monster caster)
{
    Action a;
    std::vector<std::string> spells = caster.getSpells();
    std::vector<std::string> possibleSpells;
    for (int i = 0; i < spells.size(); i++)
    {
        rapidjson::Value &spellInfo = spellBase[spells[i].c_str()];
        if (spellInfo["dealDamage"].GetBool())
        {
            if (spellInfo["manaCost"].GetInt() <= caster.mp)
                possibleSpells.push_back(spells[i]);
        }
    }
    std::string spell = possibleSpells[getRand(0, possibleSpells.size())];
    a = createAction(caster.getInfos("id"), spell, getTargetTactic(caster, spell));
    return a;
}

void Fight::spellTacticChoice(Monster &caster)
{
    // Temporaire
    if (caster.getTactic() == "Sans pitie")
    {
        actions.push_back(tacticSansPitie(caster));
        return;
    }
    if (caster.getTactic() == "Agir avec sagesse")
    {
        actions.push_back(tacticSagesse(caster));
        return;
    }
    if (caster.getTactic() == "Soins avant tout")
    {
        actions.push_back(tacticSoin(caster));
        return;
    }
    if (caster.getTactic() == "Pas de magie")
    {
        actions.push_back(tacticNoMana(caster));
        return;
    }
}

Monster &Fight::getMonsterById(std::string id)
{
    for (int i = 0; i < team1.size(); i++)
    {
        if (team1[i].getInfos("id") == id)
        {
            return team1[i];
        }
    }
    for (int i = 0; i < team2.size(); i++)
    {
        if (team2[i].getInfos("id") == id)
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

std::queue<std::string> Fight::initTurn()
{
    std::vector<std::string> alreadyAction;
    std::queue<std::string> messages;
    std::queue<std::string> messagesTotal;
    for (int i = 0; i < actions.size(); i++)
    {
        alreadyAction.push_back(actions[i].idCaster);
    }
    for (int i = 0; i < team1.size(); i++)
    {
        if (team1[i].hp > 0)
        {
            messages = team1[i].updateStatStatus();
            while (!messages.empty())
            {
                messagesTotal.push(messages.front());
                messages.pop();
            }
            if (std::count(alreadyAction.begin(), alreadyAction.end(), team1[i].getInfos("id")) == 0)
                spellTacticChoice(team1[i]);
        }
    }
    for (int i = 0; i < team2.size(); i++)
    {
        if (team2[i].hp > 0)
        {
            messages = team2[i].updateStatStatus();
            while (!messages.empty())
            {
                messagesTotal.push(messages.front());
                messages.pop();
            }
            if (std::count(alreadyAction.begin(), alreadyAction.end(), team2[i].getInfos("id")) == 0)
                spellTacticChoice(team2[i]);
        }
    }
    createStackAgility();
    return messagesTotal;
}

void Fight::checkTargetAlive(std::vector<std::string> &targets, std::string spell, Monster caster)
{
    for (int i = 0; i < targets.size(); i++)
    {
        if (getMonsterById(targets[i]).hp == 0)
        {
            targets.erase(targets.begin() + i);
        }
    }
    if (targets.size() == 0)
    {
        targets = getTargetTactic(caster, spell);
    }
}

std::string Fight::getStatusName(std::string status)
{
    if (status == "poison")
        return " est empoisonné";
    if (status == "sleep")
        return " est endormi";
    if (status == "paralysis")
        return " est paralysé";
    if (status == "confusion")
        return " est confus";
    if (status == "regen")
        return " se régénère à chaque tour";
    else
        return " non défini";
}

std::string Fight::getStatsName(std::string stats, bool debuff)
{
    if (stats == "atk")
    {
        if (debuff)
            return "L'attaque de {} diminue";
        else
            return "L'attaque de {} augmente";
    }
    if (stats == "def")
    {
        if (debuff)
            return "La défense de {} diminue";
        else
            return "La défense de {} augmente";
    }
    if (stats == "wis")
    {
        if (debuff)
            return "La sagesse de {} diminue";
        else
            return "La sagesse de {} augmente";
    }
    else
    {
        if (debuff)
            return "L'agilité de {} diminue";
        else
            return "L'agilité de {} augmente";
    }
}

std::queue<spellImpact> Fight::simulateAttack(Monster &caster, std::vector<std::string> idTargets, std::string spell)
{
    float dmg;
    spellImpact impact;
    spellImpact impactKo;
    rapidjson::Value &spellInfo = spellBase[spell.c_str()];
    std::queue<spellImpact> queueImpact;
    for (int i = 0; i < idTargets.size(); i++)
    {
        impact.function = damage;
        std::string type = spellInfo["type"].GetString();
        float statAtk = caster.getStat("atk");
        if (type != "physical")
        {
            statAtk = caster.getStat("wis");
        }
        dmg = (statAtk / 2 - getMonsterById(idTargets[i]).getStat("def") / 4) * spellInfo["damage"].GetFloat();
        if (dmg < 0)
            dmg = 0;
        const int chance = 15;
        if (getRand(0, 1000) < chance)
        {
            dmg = dmg * 2;
            spellImpact critique;
            critique.message = "C'est un coup critique !";
            queueImpact.push(critique);
        }
        if (spellInfo.HasMember("element"))
            dmg = dmg * getMonsterById(idTargets[i]).getResistance(spellInfo["element"].GetString());
        if (dmg < 0)
            impact.function = heal;
        impact.targetId.push_back(idTargets[i]);
        impact.argumentFloat.push_back(dmg);
        if (getMonsterById(idTargets[i]).hp - dmg < 1)
        {
            impactKo.message = getMonsterById(idTargets[i]).getName() + " est K.O.";
        }
    }
    if (idTargets.size() > 1)
        impact.message = "Les monstres adverses subissent " + std::to_string((int)dmg) + " points de dégâts";
    else if (dmg == 0)
        impact.message = caster.getName() + " rate son attaque";
    else
    {
        impact.message = getMonsterById(idTargets[0]).getName();
        if (dmg < 0)
            impact.message += " se soigne de " + std::to_string((int)dmg * -1) + " points de vie";
        else
            impact.message += " subit " + std::to_string((int)dmg) + " points de dégâts";
    }

    queueImpact.push(impact);
    queueImpact.push(impactKo);
    return queueImpact;
}

bool Fight::consumeMana(Monster &caster, std::string spell)
{
    rapidjson::Value &spellInfo = spellBase[spell.c_str()];
    if (caster.mp >= spellInfo["manaCost"].GetInt())
    {
        caster.mp -= spellInfo["manaCost"].GetInt();
        return true;
    }
    return false;
}

std::queue<spellImpact> Fight::getStatusEffect(Monster &caster, bool &canMove)
{
    std::queue<spellImpact> queueImpact;
    if (caster.getStatus("poison"))
    {
        spellImpact impact;
        float dmg = caster.getStat("hp") * 0.05;
        std::vector<std::string> args;
        args.push_back(caster.getName());
        args.push_back(std::to_string((int)dmg));
        impact.message = formatString("{} subit {} points de dégâts de poison", args);
        impact.function = damage;
        impact.targetId.push_back(caster.getInfos("id"));
        impact.argumentFloat.push_back(dmg);
        queueImpact.push(impact);
    }
    if (caster.getStatus("sleep"))
    {
        spellImpact impact;
        std::vector<std::string> args;
        args.push_back(caster.getName());
        impact.message = formatString("{} est endormi, il ne peut pas attaquer", args);
        impact.function = null;
        queueImpact.push(impact);
        canMove = false;
    }
    if (caster.getStatus("paralysis"))
    {
        spellImpact impact;
        std::vector<std::string> args;
        args.push_back(caster.getName());
        impact.message = formatString("{} est paralysé, il ne peut pas attaquer", args);
        impact.function = null;
        queueImpact.push(impact);
        canMove = false;
    }
    if (caster.getStatus("confusion"))
    {
        spellImpact impact;
        std::vector<std::string> args;
        args.push_back(caster.getName());
        impact.message = formatString("{} est confus", args);
        impact.function = null;
        queueImpact.push(impact);
    }
    if (caster.getStatus("regen"))
    {
        spellImpact impact;
        float valHeal = caster.getStat("hp") * 0.05;
        std::vector<std::string> args;
        args.push_back(caster.getName());
        args.push_back(std::to_string((int)valHeal));
        impact.message = formatString("{} se soigne de {} points de vie", args);
        impact.function = heal;
        impact.targetId.push_back(caster.getInfos("id"));
        impact.argumentFloat.push_back(valHeal);
        queueImpact.push(impact);
    }
    return queueImpact;
}

std::queue<spellImpact> Fight::simulateAction()
{
    assert(actionsOrdered.size() > 0);
    Action a = actionsOrdered.front();
    actionsOrdered.pop();
    if (a.spell == "null")
        return std::queue<spellImpact>();
    Monster &caster = getMonsterById(a.idCaster);
    if (caster.hp == 0)
        return std::queue<spellImpact>();
    checkTargetAlive(a.idTargets, a.spell, caster);
    if (a.idTargets.size() == 0)
        return std::queue<spellImpact>();
    spellImpact impact;
    bool multipleTarget = a.idTargets.size() > 1;
    bool canMove = true;
    std::queue<spellImpact> queueImpact = getStatusEffect(caster, canMove);
    ;
    if (!canMove)
        return queueImpact;
    rapidjson::Value &spellInfo = spellBase[a.spell.c_str()];
    if (!consumeMana(caster, a.spell))
    {
        impact.message = caster.getName() + " n'a pas assez de mana pour lancer " + a.spell;
        queueImpact.push(impact);
        return queueImpact;
    }
    std::vector<std::string> monsterName;
    monsterName.push_back(caster.getName());
    if (a.idTargets[0] == caster.getInfos("id"))
        monsterName.push_back("sur lui-même");
    else
        monsterName.push_back(getMonsterById(a.idTargets[0]).getName());
    impact.message = formatString(spellInfo["castSentence"].GetString(), monsterName);
    queueImpact.push(impact);
    std::string type = spellInfo["type"].GetString();
    if (spellInfo["dealDamage"].GetBool())
    {
        std::queue<spellImpact> tmpQueue = simulateAttack(caster, a.idTargets, a.spell);
        while (!tmpQueue.empty())
        {
            queueImpact.push(tmpQueue.front());
            tmpQueue.pop();
        }
    }
    else
    {
        if (type == "heal")
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
    if (spellInfo.HasMember("affectStatus"))
    {
        rapidjson::Value &statusJson = spellInfo["status"];
        for (rapidjson::Value::ConstMemberIterator itr = statusJson.MemberBegin(); itr != statusJson.MemberEnd(); ++itr)
        {
            for (int i = 0; i < a.idTargets.size(); i++)
            {
                float chance = 100;
                if (spellInfo["offensive"].GetBool())
                    chance = getMonsterById(a.idTargets[i]).getResistance(itr->name.GetString()) * 100;
                if (getRand(0, 100) < chance)
                {
                    spellImpact impactStatus;
                    impactStatus.message = getMonsterById(a.idTargets[i]).getName() + getStatusName(itr->name.GetString());
                    impactStatus.function = status;
                    impactStatus.targetId.push_back(a.idTargets[i]);
                    impactStatus.argumentString.push_back(itr->name.GetString());
                    impactStatus.argumentInt.push_back(itr->value.GetInt());
                    queueImpact.push(impactStatus);
                }
            }
        }
    }
    if (spellInfo.HasMember("affectStats"))
    {
        rapidjson::Value &statsJson = spellInfo["stats"];
        for (rapidjson::Value::ConstMemberIterator itr = statsJson.MemberBegin(); itr != statsJson.MemberEnd(); ++itr)
        {
            for (int i = 0; i < a.idTargets.size(); i++)
            {
                float chance = 100;
                if (spellInfo["offensive"].GetBool())
                    chance = getMonsterById(a.idTargets[i]).getResistance(itr->name.GetString()) * 100;
                if (getRand(0, 100) < chance)
                {
                    spellImpact impactStats;
                    impactStats.function = stats;
                    std::vector<std::string> name;
                    name.push_back(getMonsterById(a.idTargets[i]).getName());
                    impactStats.message = formatString(getStatsName(itr->name.GetString(), spellInfo["offensive"].GetBool()), name);
                    impactStats.targetId.push_back(a.idTargets[i]);
                    impactStats.argumentString.push_back(itr->name.GetString());
                    impactStats.argumentFloat.push_back(itr->value.GetFloat());
                    impactStats.argumentInt.push_back(spellInfo["effectTurn"].GetInt());
                    queueImpact.push(impactStats);
                }
                else
                {
                    spellImpact impactStats;
                    impactStats.function = null;
                    std::vector<std::string> name;
                    name.push_back(getMonsterById(a.idTargets[i]).getName());
                    impactStats.message = formatString("Ça n'affecte pas les statistiques de {}", name);
                    queueImpact.push(impactStats);
                }
            }
        }
    }
    return queueImpact;
}

std::queue<std::string> Fight::simulateTurn()
{
    std::queue<std::string> messages = initTurn();
    bool team1Win = true;
    while (!actionsOrdered.empty() && !isOver(team1Win))
    {
        std::queue<spellImpact> impacts = simulateAction();
        while (!impacts.empty())
        {
            messages.push(impacts.front().message);
            updateMonster(impacts.front());
            impacts.pop();
        }
    }
    return messages;
}

void Fight::updateMonster(spellImpact impact)
{
    for (int i = 0; i < impact.targetId.size(); i++)
    {
        Monster &monster = getMonsterById(impact.targetId[i]);
        if (impact.function == damage)
        {
            monster.damage(impact.argumentFloat[i]);
        }

        else if (impact.function == heal)
        {
            monster.heal(impact.argumentFloat[i]);
        }

        else if (impact.function == status)
        {
            monster.setStatus(impact.argumentString[i], impact.argumentInt[i]);
        }

        else if (impact.function == stats)
        {
            monster.setStats(impact.argumentString[i], impact.argumentFloat[i], impact.argumentInt[i]);
        }
        else
            return;
    }
}

bool Fight::isOver(bool &team1Win)
{
    int nbTeam1 = 0;
    int nbTeam2 = 0;
    for (int i = 0; i < team1.size(); i++)
    {
        if (team1[i].hp > 0)
            nbTeam1++;
    }
    for (int i = 0; i < team2.size(); i++)
    {
        if (team2[i].hp > 0)
            nbTeam2++;
    }
    if (nbTeam1 == 0)
    {
        team1Win = false;
        return true;
    }
    else if (nbTeam2 == 0)
    {
        team1Win = true;
        return true;
    }
    return false;
}

bool Fight::flee()
{
    int agilityTeam1 = 0;
    for (int i = 0; i < team1.size(); i++)
    {
        if (team1[i].hp > 0)
        {
            agilityTeam1 = team1[i].getAgility() * 100;
        }
    }
    int agilityTeam2 = 0;
    for (int i = 0; i < team2.size(); i++)
    {
        if (team2[i].hp > 0)
        {
            agilityTeam2 = team2[i].getAgility();
        }
    }
    return getRand(0, 100) < (agilityTeam1 / agilityTeam2)/2;
}
