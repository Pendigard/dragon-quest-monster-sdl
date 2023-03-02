

#include "Monster.h"

Monster::Monster(rapidjson::Value& monsterData, std::string idM, rapidjson::Document& monsterBase)
{
    id = idM;
    hp = monsterData["hp"].GetFloat();
    mp = monsterData["mp"].GetFloat();
    name = monsterData["name"].GetString();
    type = monsterData["type"].GetString();
    exp = monsterData["exp"].GetInt();
    rank = monsterBase[monsterData["type"].GetString()]["rank"].GetString();
    family = monsterBase[monsterData["type"].GetString()]["family"].GetString();
    synthLevel = monsterData["synthLevel"].GetInt();
    getLevelXp();
    //Boucle permettant de parcourir la section stats du json clé après clé
    for (rapidjson::Value::ConstMemberIterator itr = monsterData["stats"].MemberBegin(); itr != monsterData["stats"].MemberEnd(); ++itr)
    {
        stats[itr->name.GetString()] = itr->value.GetFloat();
    }
    //Boucle permettant de parcourir la section resistances du json clé après clé
    for (rapidjson::Value::ConstMemberIterator itr = monsterBase[monsterData["type"].GetString()]["resistances"].MemberBegin(); itr != monsterBase[monsterData["type"].GetString()]["resistances"].MemberEnd(); ++itr)
    {
        resistances[itr->name.GetString()] = itr->value.GetFloat();
    }
}

void Monster::getLevelXp()
{
    level = (int)std::cbrt(exp);
}

void Monster::afficher() const
{
    std::cout<<"======================"<<std::endl;
    std::cout << "Nom : " << name;
    std::cout << " Type : " << type <<std::endl;
    std::cout << "Rang : " << rank;
    if (synthLevel != 0) {std::cout << "+" << synthLevel;}
    std::cout << " Famille : " << family;
    std::cout  << std::endl << "HP : " << (int) hp <<"/"<< stats.at("hp");
    std::cout << " MP : " << (int) mp <<"/"<<stats.at("mp")<<std::endl;
    std::cout << "Niveau : " << level;
    std::cout << " Exp : " << exp << "/" << std::pow(level+1,3) << std::endl;
    std::cout<<"======================"<<std::endl;
}