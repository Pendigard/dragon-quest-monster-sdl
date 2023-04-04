#include "Database.h"

Database::Database()
{
    loadDatabase();
}

void Database::loadDatabase()
{
    loadJson("data/database/monster.json",monsterBase);
    loadJson("data/database/monsterLibrary.json",library);
    loadJson("data/database/skill.json",skillBase);
    loadJson("data/database/spell.json",spellBase);
    loadJson("data/database/synthesisRecipe.json",synthesisRecipe);
}

rapidjson::Document& Database::getMonsterBase()
{
    return monsterBase;
}

rapidjson::Document& Database::getLibrary()
{
    return library;
}

rapidjson::Document& Database::getSkillBase()
{
    return skillBase;
}

rapidjson::Document& Database::getSpellBase()
{
    return spellBase;
}

rapidjson::Value& Database::getMonsterStat(std::string family, std::string rank, std::string type)
{
    return monsterBase[family.c_str()][rank.c_str()][type.c_str()];
}

rapidjson::Document& Database::getSynthesisRecipe()
{
    return synthesisRecipe;
}
