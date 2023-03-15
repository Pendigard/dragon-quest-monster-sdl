

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

int getXpLvl(int level)
{
    int xp = 1;
    for (int i = 1; i < level; i++)
    {
        xp += std::pow(i, 3);
    }
    return xp;
}

Monster::Monster(rapidjson::Value &monsterData, std::string idM, rapidjson::Document &monsterBase, rapidjson::Document &skillBase, rapidjson::Document &library)
{
    tactic = "Sans pitie";
    std::string type = monsterData["type"].GetString();
    std::string family = library[type.c_str()]["family"].GetString();
    std::string rank = library[type.c_str()]["rank"].GetString();
    synthId = library[type.c_str()]["id"].GetInt();
    rapidjson::Value &monsterStatBase = monsterBase[family.c_str()][rank.c_str()][type.c_str()];
    jsonToUnorderedMap(monsterStatBase["statMax"], statMax);
    hp = monsterData["hp"].GetFloat();
    mp = monsterData["mp"].GetFloat();
    infos["id"] = idM;
    infos["name"] = monsterData["name"].GetString();
    infos["type"] = type;
    infos["rank"] = rank;
    infos["family"] = family;
    exp = monsterData["exp"].GetInt();
    level = 1; 
    while (getXpLvl(level+1) < exp)
    {
        level++;
    }
    synthLevel = monsterData["synthLevel"].GetInt();
    skillPoints = monsterData["skillPoints"].GetInt();
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

Monster::Monster(std::string name, std::string type, rapidjson::Document &monsterBase, rapidjson::Document &skillBase, rapidjson::Document &save, rapidjson::Document &library, unsigned int lvl = 1)
{
    tactic = "Sans pitie";
    do
    {
        infos["id"] = generateID();
    } while (save.HasMember(infos["id"].c_str()));
    std::string family = library[type.c_str()]["family"].GetString();
    std::string rank = library[type.c_str()]["rank"].GetString();
    synthId = library[type.c_str()]["id"].GetInt();
    rapidjson::Value &monsterStatBase = monsterBase[family.c_str()][rank.c_str()][type.c_str()];
    infos["name"] = name;
    infos["type"] = type;
    infos["rank"] = rank;
    infos["family"] = family;
    hp = monsterStatBase["growth"]["hp"].GetFloat();
    mp = monsterStatBase["growth"]["mp"].GetFloat();
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
    exp = 0;
    addXp(getXpLvl(lvl));
    autoAttributeSkill(skillBase);
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

int Monster::getMaxLvl() const
{
    if (synthLevel < 5)
    {
        return 50;
    }
    else if (synthLevel < 10)
    {
        return 75;
    }
    return 100;
}

void Monster::levelUp()
{
    int oldLevel = level;
    level++;
    for (auto &stat : stats)
    {
        stat.second += std::min(growth.at(stat.first), statMax.at(stat.first) - stat.second);
    }
    for (long unsigned int i = oldLevel; i < level; i++)
    {
        if (i % 5 == 0)
            skillPoints += 5;
    }
    hp = stats.at("hp");
    mp = stats.at("mp");
}

void Monster::addXp(unsigned int xp)
{
    if (level == getMaxLvl())
    {
        return;
    }
    exp += xp;
    while (getXpLvl(level+1) < exp)
    {
        levelUp();
    }
}

void Monster::addSkillPoint(unsigned int points)
{
    skillPoints += points;
}

void Monster::applySkillPoint(unsigned int points, std::string skill, rapidjson::Document &skillBase)
{
    points = std::min(points, skillPoints);
    unsigned int maxPointSkillSet = skillBase[skill.c_str()]["maxPoints"].GetInt();
    int maxPointToAttribute = std::min(maxPointSkillSet, points + skills[skill]);
    int lastSkillCap = skills[skill];
    for (rapidjson::Value::ConstMemberIterator itr = skillBase[skill.c_str()].MemberBegin() + 1; itr != skillBase[skill.c_str()].MemberEnd(); ++itr)
    {
        const rapidjson::Value &currentSkill = itr->value;
        std::string type = currentSkill["upgradeType"].GetString();
        int point = currentSkill["point"].GetInt();
        if (lastSkillCap < point && point <= maxPointToAttribute)
        {
            lastSkillCap += point;
            if (type == "spell")
            {
                spells.push_back(itr->name.GetString());
            }
        }
    }
    skillPoints -= maxPointToAttribute;
    skills[skill] = maxPointToAttribute;
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
    std::cout << "Exp : " << exp - getXpLvl(level) << "/" << std::min((int)std::pow(level + 1, 3), (int)std::pow(getMaxLvl(), 3)) << " ";
    for (int i = 0; i < ((exp - getXpLvl(level)) * 10) / std::pow(level + 1, 3); i++)
    {
        std::cout << "■";
    }
    for (int i = 0; i < 10 - (((exp - getXpLvl(level)) * 10) / std::pow(level + 1, 3)); i++)
    {
        std::cout << "□";
    }
    std::cout << std::endl;
    std::cout<<"Exp total : "<<exp<<std::endl;
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
    return infos.at("id") == monster.getInfos("id");
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

std::string Monster::getInfos(std::string info) const
{
    return infos.at(info);
}

unsigned int Monster::getSynthId() const
{
    return synthId;
}

void Monster::autoAttributeSkill(rapidjson::Document &skillBase)
{
    for (int i = 0; i < skillPoints; i += 5)
    {
        for (auto &skill : skills)
        {
            applySkillPoint(5, skill.first, skillBase);
        }
    }
}

void Monster::printSkill() const
{
    std::cout << std::endl;
    for (auto &skill : skills)
    {
        std::cout << skill.first << " : " << skill.second << " | ";
    }
    std::cout << std::endl;
    for (auto &spell : spells)
    {
        std::cout << spell << " | ";
    }
    std::cout << std::endl;
}