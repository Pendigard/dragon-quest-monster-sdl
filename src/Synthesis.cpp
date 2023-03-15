#include "Synthesis.h"

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
    if (rankMap[rank]>5)
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
        if (id == parent.getSynthId()+1)
        {
            return monster;
        }
    }
    return monster;
}

std::vector<std::string> makeSynthesisPreview(Monster parent1, Monster parent2)
{
    std::vector<std::string> result;
    std::string rank = getSynthesisRank(parent1, parent2);
    std::string monster1 = getChildMonster(parent1, rank);
    std::string monster2 = getChildMonster(parent2, rank);
    result.push_back(monster1);
    result.push_back(monster2);
    return result;
}