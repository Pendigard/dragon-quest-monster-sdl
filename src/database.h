#ifndef _DATABASE
#define _DATABASE

#include <vector>
#include "jsonFunction.h"

class Database
{
    private:
        rapidjson::Document monsterBase;
        rapidjson::Document library;
        rapidjson::Document skillBase;
        rapidjson::Document spellBase;
    
    public:
        Database();
        void loadDatabase();
        rapidjson::Document& getMonsterBase();
        rapidjson::Document& getLibrary();
        rapidjson::Document& getSkillBase();
        rapidjson::Document& getSpellBase();
        rapidjson::Value& getMonsterStat(std::string family, std::string rank, std::string type);
};

#endif