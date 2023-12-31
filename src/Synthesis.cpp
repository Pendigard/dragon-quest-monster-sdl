#include "Synthesis.h"

bool isRecipe(Monster parent1, Monster parent2, Database &db)
{
    rapidjson::Document &recipe = db.getSynthesisRecipe();
    std::string type1 = parent1.getInfos("type");
    std::string type2 = parent2.getInfos("type");
    bool test1 = (recipe.HasMember(type1.c_str())) && (recipe[type1.c_str()].HasMember(type2.c_str()));
    bool test2 = (recipe.HasMember(type2.c_str())) && (recipe[type2.c_str()].HasMember(type1.c_str()));
    return test1 && test2;
}

std::string getMonsterFromRecipe(Monster parent1, Monster parent2, Database &db)
{
    rapidjson::Document &recipe = db.getSynthesisRecipe();
    std::string type1 = parent1.getInfos("type");
    std::string type2 = parent2.getInfos("type");
    std::string result = recipe[type1.c_str()][type2.c_str()]["child"].GetString();
    return result;
}

std::string getSynthesisRank(Monster parent1, Monster parent2)
{
    std::unordered_map<std::string, int> rankMap;
    rankMap["F"] = 0;
    rankMap["E"] = 1;
    rankMap["D"] = 2;
    rankMap["C"] = 3;
    rankMap["B"] = 4;
    rankMap["A"] = 5;
    rankMap["S"] = 6;
    rankMap["X"] = 7;
    std::string rank = parent2.getInfos("rank");

    if (rankMap[parent1.getInfos("rank")] > rankMap[parent2.getInfos("rank")])
    {
        rank = parent1.getInfos("rank");
    }
    if (rankMap[rank] > 5)
        rank = "A";
    return rank;
}

std::string getChildMonster(Monster parent, std::string rank)
{
    rapidjson::Document monsterBase;
    loadJson("data/database/monster.json", monsterBase);
    const rapidjson::Value &synthChain = monsterBase[parent.getInfos("family").c_str()][rank.c_str()];
    if (parent.getInfos("rank") != rank)
        return synthChain.MemberBegin()->name.GetString();
    int id = 0;
    std::string monster;
    for (rapidjson::Value::ConstMemberIterator itr = synthChain.MemberBegin(); itr != synthChain.MemberEnd(); ++itr)
    {
        id++;
        monster = itr->name.GetString();
        if (id == parent.getSynthId() + 1)
        {
            return monster;
        }
    }
    return monster;
}

bool parent1HigherId(Monster parent1, Monster parent2, std::string rank)
{
    if (parent1.getInfos("rank") != rank)
        return true;
    else if (parent2.getInfos("rank") != rank)
        return true;
    else
    {
        if (parent1.getSynthId() > parent2.getSynthId())
            return true;
        return false;
    }
    return true;
}

std::vector<std::string> makeSynthesisPreview(Monster parent1, Monster parent2, Database &db)
{
    std::vector<std::string> result;
    
    if (isRecipe(parent1, parent2, db))
    {
        result.push_back(getMonsterFromRecipe(parent1, parent2, db));
        return result;    
    }
    if (parent1.getInfos("family") == "???") {
        result.push_back(parent1.getInfos("type"));
    }
    if (parent2.getInfos("family") == "???") {
        result.push_back(parent2.getInfos("type"));
    }
    if (parent1.getInfos("family") == "???" || parent2.getInfos("family") == "???") {
        return result;
    }
    std::string rank = getSynthesisRank(parent1, parent2);
    std::string monster1 = getChildMonster(parent1, rank);
    std::string monster2 = getChildMonster(parent2, rank);
    if (parent1.getInfos("family") == parent2.getInfos("family"))
    {
        if (parent1HigherId(parent1, parent2, rank))
        {
            result.push_back(monster1);
            return result;
        }
        result.push_back(monster2);
        return result;
    }
    result.push_back(monster1);
    result.push_back(monster2);
    return result;
}

std::unordered_map<std::string, unsigned int> getSynthesisSkills(Monster parent1, Monster parent2, std::string child, Database &db)
{
    std::unordered_map<std::string, unsigned int> possibleSkill;
    std::vector<std::string> alreadyUsed;
    rapidjson::Document &skillBase = db.getSkillBase();
    for (auto &skill : parent1.getSkills())
    {
        alreadyUsed.push_back(skill.first);
        int points = skill.second;
        if (parent2.getSkills().find(skill.first) != parent2.getSkills().end())
        {
            points += parent2.getSkills()[skill.first];
        }
        if (skillBase[skill.first.c_str()].HasMember("setUpgrade"))
        {
            if (skillBase[skill.first.c_str()]["maxPoints"].GetInt() < points)
                possibleSkill[skillBase[skill.first.c_str()]["setUpgrade"].GetString()] = 0;
        }
        else
        {
            possibleSkill[skill.first] = points / 2;
        }
    }
    for (auto &skill : parent2.getSkills())
    {
        if (std::find(alreadyUsed.begin(), alreadyUsed.end(), skill.first) == alreadyUsed.end())
        {
            int points = skill.second;
            if (skillBase[skill.first.c_str()].HasMember("setUpgrade"))
            {
                if (skillBase[skill.first.c_str()]["maxPoints"].GetInt() < points)
                    possibleSkill[skillBase[skill.first.c_str()]["setUpgrade"].GetString()] = 0;
            }
            else
            {
                possibleSkill[skill.first] = points / 2;
            }
        }
        alreadyUsed.push_back(skill.first);
    }
    rapidjson::Document &lib = db.getLibrary();
    std::string childSkill = db.getMonsterStat(lib[child.c_str()]["family"].GetString(), lib[child.c_str()]["rank"].GetString(), child)["skills"].GetString();
    if (std::find(alreadyUsed.begin(), alreadyUsed.end(), childSkill) == alreadyUsed.end())
    {
        possibleSkill[childSkill] = 0;
    }
    return possibleSkill;
}