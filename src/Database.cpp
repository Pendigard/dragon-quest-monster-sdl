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
    loadJson("data/scripts/scripts.json",scripts);

    loadJson("data/tileset/info/ground_1.json",tilesets["../tileset/info/ground_1.json"]);
    loadJson("data/tileset/info/ground_2.json",tilesets["../tileset/info/ground_2.json"]);
    loadJson("data/tileset/info/ground_3.json",tilesets["../tileset/info/ground_3.json"]);
    loadJson("data/tileset/info/cliff_1.json",tilesets["../tileset/info/cliff_1.json"]);
    loadJson("data/tileset/info/cliff_2.json",tilesets["../tileset/info/cliff_2.json"]);
    loadJson("data/tileset/info/assets_1.json",tilesets["../tileset/info/assets_1.json"]);
    loadJson("data/tileset/info/assets_2.json",tilesets["../tileset/info/assets_2.json"]);
    loadJson("data/tileset/info/assets_3.json",tilesets["../tileset/info/assets_3.json"]);
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

rapidjson::Value& Database::getMonsterStatType(std::string type)
{
    return monsterBase[library[type.c_str()]["family"].GetString()][library[type.c_str()]["rank"].GetString()][type.c_str()];
}

rapidjson::Value& Database::getMonsterStat(std::string family, std::string rank, std::string type)
{
    return monsterBase[family.c_str()][rank.c_str()][type.c_str()];
}

rapidjson::Document& Database::getSynthesisRecipe()
{
    return synthesisRecipe;
}

int Database::getTileSetWidth(std::string tileset) const
{
    
    return tilesets.at(tileset)["imagewidth"].GetInt();
}

int Database::getTileSetHeight(std::string tileset) const
{
    return tilesets.at(tileset)["imageheight"].GetInt();
}

int Database::getTileWidth(std::string tileset) const
{
    return tilesets.at(tileset)["tilewidth"].GetInt();
}

int Database::getTileHeight(std::string tileset) const
{
    return tilesets.at(tileset)["tileheight"].GetInt();
}

std::string Database::getImage(std::string tileset) const
{
    return tilesets.at(tileset)["image"].GetString();
}

rapidjson::Value& Database::getScript(std::string scriptName)
{
    return scripts[scriptName.c_str()];
}
