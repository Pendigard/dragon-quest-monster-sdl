

#include "Monster.h"
#include <cstdlib>
#include <ctime>

int getRand(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

std::string Monster::generateID() const
{
    std::string carac = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string id = "";
    for (int i = 0; i < 8; i++)
    {
        id += carac[getRand(0, carac.size() - 1)];
    }
    return id;
}

Monster::Monster(rapidjson::Value &monsterData, std::string idM, rapidjson::Document& monsterBase, rapidjson::Document &skillBase)
{
    tactic = "Sans pitié";
    jsonToUnorderedMap(monsterBase[monsterData["type"].GetString()]["statMax"],statMax);
    hp = monsterData["hp"].GetFloat();
    mp = monsterData["mp"].GetFloat();
    infos["id"] = idM;
    infos["name"] = monsterData["name"].GetString();
    infos["type"] = monsterData["type"].GetString();
    infos["rank"] = monsterBase[monsterData["type"].GetString()]["rank"].GetString();
    infos["family"] = monsterBase[monsterData["type"].GetString()]["family"].GetString();
    exp = monsterData["exp"].GetInt();
    synthLevel = monsterData["synthLevel"].GetInt();
    skillPoints = monsterData["skillPoints"].GetInt();
    getLevelXp();
    jsonToUnorderedMap(monsterData["skills"], skills);
    jsonToUnorderedMap(monsterData["stats"], stats);
    jsonToUnorderedMap(monsterData["growth"], growth);
    jsonToUnorderedMap(monsterBase[monsterData["type"].GetString()]["resistances"], resistances);
    updateSpell(skillBase);
    for (auto &stat : stats)
    {
        alterationsTurn[stat.first] = 0;
        alterations[stat.first] = 1;
    }
    initStatus();
}

Monster::Monster(std::string name, std::string type,rapidjson::Document &monsterBase, rapidjson::Document &skillBase, rapidjson::Document &save,unsigned int lvl=1)
{
    tactic = "Sans pitié";
    do {
        infos["id"] = generateID();
    } while (save.HasMember(infos["id"].c_str()));
    infos["name"] = name;
    infos["type"] = type;
    infos["rank"] = monsterBase[type.c_str()]["rank"].GetString();
    infos["family"] = monsterBase[type.c_str()]["family"].GetString();
    hp = monsterBase[type.c_str()]["growth"]["hp"].GetFloat();
    mp = monsterBase[type.c_str()]["growth"]["mp"].GetFloat();
    exp = 1;
    synthLevel = 0;
    skillPoints = 0;
    level = 1;
    skills[monsterBase[type.c_str()]["skills"].GetString()] = 0;
    jsonToUnorderedMap(monsterBase[type.c_str()]["statMax"],statMax);
    jsonToUnorderedMap(monsterBase[type.c_str()]["growth"], stats);
    jsonToUnorderedMap(monsterBase[type.c_str()]["growth"], growth);
    jsonToUnorderedMap(monsterBase[type.c_str()]["resistances"], resistances);
    updateSpell(skillBase);
    for (auto &stat : stats)
    {
        alterationsTurn[stat.first] = 0;
        alterations[stat.first] = 1;
    }
    initStatus();
    addXp(std::pow(lvl,3));
    createSaveMonster(save);
}

void Monster::initStatus()
{
    status["poison"] = 0;
    status["paralysie"] = 0;
    status["sommeil"] = 0;
    status["confusion"] = 0;
    status["aveugle"] = 0;
    status["regenerationVie"] = 0;
    status["regenerationMana"] = 0;
}

int Monster::getMaxXp() const
{
    if (synthLevel < 5) {
        return std::pow(50,3);
    }
    else if (synthLevel < 10)
    {
        return std::pow(75,3);
    }
    return std::pow(100,3);
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
    hp = stats.at("hp");
    mp = stats.at("mp");
}

void Monster::updateSpell(rapidjson::Document &skillBase)
{
    spells.clear();
    spells.push_back("Attaque");
    spells.push_back("Defense");
    for (auto &skill : skills)
    {
        for (rapidjson::Value::ConstMemberIterator itr = skillBase[skill.first.c_str()].MemberBegin(); itr != skillBase[skill.first.c_str()].MemberEnd(); ++itr)
        {
            if (itr->value.GetInt() <= skill.second && itr != skillBase[skill.first.c_str()].MemberBegin())
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
    unsigned int maxSkillSet = skillBase[skill.c_str()]["maxPoints"].GetInt() - skills[skill];
    skills[skill] += std::min(std::min(points, skillPoints), maxSkillSet);
    skillPoints -= std::min(std::min(points, skillPoints), maxSkillSet);
    updateSpell(skillBase);
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

    std::cout << "Exp : " << exp << "/" << std::min((int)std::pow(level + 1,3), getMaxXp()) << " ";
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
    for (int i = 0; i < spells.size(); i++)
    {
        std::cout <<i+1<<". "<< spells[i] << std::endl;
    }
}

std::string Monster::getId() const
{
    return infos.at("id");
}

void Monster::updateSaveMonster(rapidjson::Document &save) const
{
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
    return stats.at(stat);
}

void Monster::damage(float damage)
{
    hp -= damage;
    if (hp < 1)
    {
        hp = 0;
    }
}

void Monster::heal(float heal)
{
    hp += heal;
    if (hp > stats.at("hp"))
    {
        hp = stats.at("hp");
    }
}