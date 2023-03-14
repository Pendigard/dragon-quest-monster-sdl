

#include "Monster.h"
#include <cstdlib>
#include <ctime>

std::string Monster::generateID() const
{
    std::string carac = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string id = "";
    for (int i = 0; i < 8; i++)
    {
        id += carac[getRand(0, carac.size())];
    }
    return id;
}

Monster::Monster(rapidjson::Value &monsterData, std::string idM, rapidjson::Document &monsterBase, rapidjson::Document &skillBase,rapidjson::Document& library)
{
    tactic = "Sans pitie";
    std::string type = monsterData["type"].GetString();
    std::string family =library[type.c_str()]["family"].GetString();
    std::string rank = library[type.c_str()]["rank"].GetString();
    rapidjson::Value& monsterStatBase = monsterBase[family.c_str()][rank.c_str()][type.c_str()];
    jsonToUnorderedMap(monsterStatBase["statMax"], statMax);
    hp = monsterData["hp"].GetFloat();
    mp = monsterData["mp"].GetFloat();
    infos["id"] = idM;
    infos["name"] = monsterData["name"].GetString();
    infos["type"] = type;
    infos["rank"] = rank;
    infos["family"] = family;
    exp = monsterData["exp"].GetInt();
    synthLevel = monsterData["synthLevel"].GetInt();
    skillPoints = monsterData["skillPoints"].GetInt();
    getLevelXp();
    jsonToUnorderedMap(monsterData["skills"], skills);
    jsonToUnorderedMap(monsterData["stats"], stats);
    jsonToUnorderedMap(monsterData["growth"], growth);
    jsonToUnorderedMap(monsterStatBase["resistances"], resistances);
    jsonToVector(monsterData["spells"], spells);
    for (auto &stat : stats)
    {
        alterationsTurn[stat.first] = 0;
        alterations[stat.first] = 1;
    }
    initStatus();
}

Monster::Monster(std::string name, std::string type, rapidjson::Document &monsterBase, rapidjson::Document &skillBase, rapidjson::Document &save, rapidjson::Document& library,unsigned int lvl = 1)
{
    tactic = "Sans pitie";
    do
    {
        infos["id"] = generateID();
    } while (save.HasMember(infos["id"].c_str()));
    std::string family =library[type.c_str()]["family"].GetString();
    std::string rank = library[type.c_str()]["rank"].GetString();
    rapidjson::Value& monsterStatBase = monsterBase[family.c_str()][rank.c_str()][type.c_str()];
    infos["name"] = name;
    infos["type"] = type;
    infos["rank"] = rank;
    infos["family"] = family;
    hp = monsterStatBase["growth"]["hp"].GetFloat();
    mp = monsterStatBase["growth"]["mp"].GetFloat();
    exp = 1;
    synthLevel = 0;
    skillPoints = 0;
    level = 1;
    skills[monsterStatBase["skills"].GetString()] = 0;
    jsonToUnorderedMap(monsterStatBase["statMax"], statMax);
    jsonToUnorderedMap(monsterStatBase["growth"], stats);
    jsonToUnorderedMap(monsterStatBase["growth"], growth);
    jsonToUnorderedMap(monsterStatBase["resistances"], resistances);
    spells.push_back("Attaque");
    spells.push_back("Defense");
    for (auto &stat : stats)
    {
        alterationsTurn[stat.first] = 0;
        alterations[stat.first] = 1;
    }
    initStatus();
    addXp(std::pow(lvl, 3));
    //createSaveMonster(save);
}

void Monster::initStatus()
{
    status["poison"] = 0;
    status["paralysis"] = 0;
    status["sleep"] = 0;
    status["confusion"] = 0;
    status["regen"] = 0;
    status["regenerationMana"] = 0;
}

int Monster::getMaxXp() const
{
    if (synthLevel < 5)
    {
        return std::pow(50, 3);
    }
    else if (synthLevel < 10)
    {
        return std::pow(75, 3);
    }
    return std::pow(100, 3);
}

void Monster::getLevelXp()
{
    level = (int)std::cbrt(exp);
}

void Monster::levelUp(unsigned int level)
{
    for (auto &stat : stats)
    {
        stat.second += std::min(growth.at(stat.first) * (level - this->level), statMax.at(stat.first) - stat.second);
    }
    this->level = level;
    if (level % 5 == 0)
    {
        skillPoints += 5;
    }
    hp = stats.at("hp");
    mp = stats.at("mp");
}

void Monster::getSpell(rapidjson::Document &skillBase)
{
    spells.clear();
    spells.push_back("Attaque");
    spells.push_back("Defense");
    for (auto &skill : skills)
    {
        for (rapidjson::Value::ConstMemberIterator itr = skillBase[skill.first.c_str()].MemberBegin(); itr != skillBase[skill.first.c_str()].MemberEnd(); ++itr)
        {
            if (itr == skillBase[skill.first.c_str()].MemberBegin())
            {
                itr++;
            }
            const rapidjson::Value &currentSkill = itr->value;
            int point = currentSkill["point"].GetInt();
            std::string type = currentSkill["upgradeType"].GetString();
            if (point <= skill.second && type == "spell")
            {
                spells.push_back(itr->name.GetString());
            }
        }
    }
}

void Monster::addXp(unsigned int xp)
{
    exp += std::min(xp, getMaxXp() - exp);
    unsigned int level = (int)std::cbrt(exp);
    if (level > this->level)
    {
        levelUp(level);
    }
}

void Monster::addSkillPoint(unsigned int points)
{
    skillPoints += points;
}

void Monster::applySkillPoint(unsigned int points, std::string skill, rapidjson::Document &skillBase)
{
    skillPoints -= points;
    int tmpPoint;
    unsigned int pointUpgraded = skills[skill];     // point deja upgradé
    int pointLeft = points;                // point restant a upgradé
    unsigned int pointMax = points + skills[skill]; // point max a upgradé
    for (rapidjson::Value::ConstMemberIterator itr = skillBase[skill.c_str()].MemberBegin(); itr != skillBase[skill.c_str()].MemberEnd(); ++itr)
    {
        if (itr == skillBase[skill.c_str()].MemberBegin())
        {
            itr++;
        }
        const rapidjson::Value &currentSkill = itr->value;
        tmpPoint = currentSkill["point"].GetInt();
        std::string type = currentSkill["upgradeType"].GetString();
        pointLeft = tmpPoint - pointUpgraded;
        if (tmpPoint <= pointMax && pointUpgraded < tmpPoint)
        {
            if (type == "spell")
            {
                spells.push_back(itr->name.GetString());
                pointUpgraded = tmpPoint;
            }
        }
    }
    pointLeft = tmpPoint - pointUpgraded;
    skills[skill] = points - pointLeft;
    skillPoints += pointLeft;
}

void Monster::print() const
{
    std::cout << "===================================" << std::endl;
    std::cout << "Nom : " << infos.at("name") << " Niveau : " << level;
    std::cout << " Rang : " << infos.at("rank");
    if (synthLevel != 0)
    {
        std::cout << "+" << synthLevel;
    }
    std::cout << std::endl
              << "Type : " << infos.at("type");
    std::cout << " Famille : " << infos.at("family") << std::endl;
    std::cout << "HP : " << (int)hp << "/" << (int)stats.at("hp");
    std::cout << " MP : " << (int)mp << "/" << (int)stats.at("mp") << std::endl;
    std::cout << "Attaque : " << (int)stats.at("atk") << " Defense : " << (int)stats.at("def") << std::endl;
    std::cout << "Agilité : " << (int)stats.at("agi") << " Sagesse : " << (int)stats.at("wis") << std::endl;

    std::cout << "Exp : " << exp << "/" << std::min((int)std::pow(level + 1, 3), getMaxXp()) << " ";
    for (int i = 0; i < 10 - (pow(level + 1, 3) - exp) / (pow(level + 1, 3) - pow(level, 3)) * 10; i++)
    {
        std::cout << "■";
    }
    for (int i = 0; i < (pow(level + 1, 3) - exp) / (pow(level + 1, 3) - pow(level, 3)) * 10; i++)
    {
        std::cout << "□";
    }
    std::cout << std::endl;
    std::cout << "===================================" << std::endl;
}

void Monster::printSpells() const
{
    rapidjson::Document spellBase;
    loadJson("data/database/spell.json", spellBase);
    for (int i = 0; i < spells.size(); i++)
    {
        std::cout << i + 1 << ". " << spells[i];
        if (spells[i] != "Attaque" && spells[i] != "Defense")
        {
            std::cout << " " << spellBase[spells[i].c_str()]["manaCost"].GetInt() << " PM";
        }
        std::cout << std::endl;
    }
}

std::string Monster::getId() const
{
    return infos.at("id");
}

void Monster::updateSaveMonster(rapidjson::Document &save) const
{
    assert(save.HasMember("monsters"));
    assert(save["monsters"].HasMember(infos.at("id").c_str()));
    rapidjson::Value &monster = save["monsters"][infos.at("id").c_str()];
    monster["hp"] = hp;
    monster["mp"] = mp;
    monster["exp"] = exp;
    monster["skillPoints"] = skillPoints;
    for (auto &skill : skills)
    {
        monster["skills"][skill.first.c_str()] = skill.second;
    }
    for (auto &stat : stats)
    {
        monster["stats"][stat.first.c_str()] = stat.second;
    }
    rapidjson::Value &spellsJson = monster["spells"];
    spellsJson.SetArray();
    for (auto &spell : spells)
    {
        spellsJson.PushBack(rapidjson::StringRef(spell.c_str()), save.GetAllocator());
    }
}

void Monster::createSaveMonster(rapidjson::Document &save) const
{
    rapidjson::Value &monster = save["monsters"];
    rapidjson::Value monsterStat;
    monsterStat.SetObject();
    monsterStat.AddMember("name", rapidjson::StringRef(infos.at("name").c_str()), save.GetAllocator());
    monsterStat.AddMember("type", rapidjson::StringRef(infos.at("type").c_str()), save.GetAllocator());
    monsterStat.AddMember("hp", hp, save.GetAllocator());
    monsterStat.AddMember("mp", mp, save.GetAllocator());
    rapidjson::Value statsJson;
    statsJson.SetObject();
    for (auto &stat : stats)
    {
        statsJson.AddMember(rapidjson::StringRef(stat.first.c_str()), stat.second, save.GetAllocator());
    }
    monsterStat.AddMember("stats", statsJson, save.GetAllocator());
    rapidjson::Value growthJson;
    growthJson.SetObject();
    for (auto &growth : growth)
    {
        growthJson.AddMember(rapidjson::StringRef(growth.first.c_str()), growth.second, save.GetAllocator());
    }
    monsterStat.AddMember("growth", growthJson, save.GetAllocator());
    rapidjson::Value skillsJson;
    skillsJson.SetObject();
    for (auto &skill : skills)
    {
        skillsJson.AddMember(rapidjson::StringRef(skill.first.c_str()), (int)skill.second, save.GetAllocator());
    }
    rapidjson::Value spells(rapidjson::kArrayType);
    for (int i = 0; i < this->spells.size(); i++)
    {
        spells.PushBack(rapidjson::StringRef(this->spells[i].c_str()), save.GetAllocator());
    }
    monsterStat.AddMember("spells", spells, save.GetAllocator());
    monsterStat.AddMember("skills", skillsJson, save.GetAllocator());
    monsterStat.AddMember("skillPoints", skillPoints, save.GetAllocator());
    monsterStat.AddMember("exp", exp, save.GetAllocator());
    monsterStat.AddMember("synthLevel", skillPoints, save.GetAllocator());
    monster.AddMember(rapidjson::StringRef(infos.at("id").c_str()), monsterStat, save.GetAllocator());
}

bool Monster::operator==(const Monster &monster) const
{
    return infos.at("id") == monster.getId();
}

float Monster::getAgility() const
{
    return stats.at("agi");
}

unsigned int Monster::getExp() const
{
    return exp;
}

std::vector<std::string> Monster::getSpells() const
{
    return spells;
}

std::string Monster::getName() const
{
    return infos.at("name");
}

std::string Monster::getType() const
{
    return infos.at("type");
}

float Monster::getStat(std::string stat) const
{
    assert(stats.count(stat) == 1);
    return stats.at(stat) * alterations.at(stat);
}

float Monster::getResistance(std::string resistance) const
{
    assert(resistances.count(resistance) == 1);
    return resistances.at(resistance);
}

std::string Monster::getTactic() const
{
    return tactic;
}

void Monster::damage(float damage)
{
    assert(damage >= 0);
    hp -= damage;
    if (hp < 1)
    {
        hp = 0;
    }
}

void Monster::heal(float heal)
{
    if (hp == 0)
    {
        return;
    }
    hp += heal;
    if (hp > stats.at("hp"))
    {
        hp = stats.at("hp");
    }
}

void Monster::setStatus(std::string newStatus, int turns)
{
    status[newStatus] = turns;
}

void Monster::setStats(std::string stat, float value, int turns)
{
    if (value > 4)
        value = 4;
    if (value < 0.25)
        value = 0.25;
    alterations[stat] = value;
    alterationsTurn[stat] = turns;
}

std::string statStatusMsg(std::string statStatus)
{
    if (statStatus == "atk")
    {
        return "L'attaque de {} revient à la normale";
    }
    if (statStatus == "def")
    {
        return "La défense de {} revient à la normale";
    }
    if (statStatus == "agi")
    {
        return "L'agilité de {} revient à la normale";
    }
    if (statStatus == "wis")
    {
        return "La sagesse de {} revient à la normale";
    }
    if (statStatus == "poison")
    {
        return "{} n'est plus empoisonné";
    }
    if (statStatus == "sleep")
    {
        return "{} se réveille";
    }
    if (statStatus == "paralysis")
    {
        return "{} n'est plus paralysé";
    }
    if (statStatus == "confusion")
    {
        return "{} n'est plus confus";
    }
    if (statStatus == "regen")
    {
        return "La régénération de {} s'arrête";
    }
    return "statut inconnu";
}

std::queue<std::string> Monster::updateStatStatus()
{
    std::queue<std::string> messages;
    for (auto &statut : status)
    {
        if (statut.second > 0)
        {
            statut.second--;
            if (statut.second == 0)
            {
                std::string message = formatString(statStatusMsg(statut.first), {getName()});
                messages.push(message);
            }
        }
    }
    for (auto &alteration : alterations)
    {
        if (alterationsTurn[alteration.first] > 0)
        {
            alterationsTurn[alteration.first]--;
            if (alterationsTurn[alteration.first] == 0)
            {
                alteration.second = 1;
                std::string message = formatString(statStatusMsg(alteration.first), {getName()});
                messages.push(message);
            }
        }
    }
    return messages;
}

bool Monster::getStatus(std::string status) const
{
    return this->status.at(status) > 0;
}

void Monster::setTactic(std::string tactic)
{
    this->tactic = tactic;
}

unsigned int Monster::getSkillToAttribute() const
{
    return skillPoints;
}