

#include "Monster.h"

void jsonToUnorderedMap(rapidjson::Value& json, std::unordered_map<std::string, float>& map)
{
    //Boucle permettant de parcourir le json clé après clé
    for (rapidjson::Value::ConstMemberIterator itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr)
    {
        map[itr->name.GetString()] = itr->value.GetFloat();
    }
}

void jsonToUnorderedMap(rapidjson::Value& json, std::unordered_map<std::string, unsigned int>& map)
{
    //Boucle permettant de parcourir le json clé après clé
    for (rapidjson::Value::ConstMemberIterator itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr)
    {
        map[itr->name.GetString()] = itr->value.GetInt();
    }
}

Monster::Monster(rapidjson::Value& monsterData, std::string idM, rapidjson::Document& monsterBase)
{
    hp = monsterData["hp"].GetFloat();
    mp = monsterData["mp"].GetFloat();
    infos["id"] = idM;
    infos["name"] = monsterData["name"].GetString();
    infos["type"] = monsterData["type"].GetString();
    infos["rank"] = monsterBase[monsterData["type"].GetString()]["rank"].GetString();
    infos["family"] = monsterBase[monsterData["type"].GetString()]["family"].GetString();
    exp = monsterData["exp"].GetInt();
    synthLevel = monsterData["synthLevel"].GetInt();
    getLevelXp();
    jsonToUnorderedMap(monsterData["skills"], skills);
    jsonToUnorderedMap(monsterData["stats"], stats);
    jsonToUnorderedMap(monsterData["growth"], growth);
    jsonToUnorderedMap(monsterBase[monsterData["type"].GetString()]["resistances"], resistances);
}

void Monster::getLevelXp()
{
    level = (int)std::cbrt(exp);
}

void Monster::afficher() const
{
    std::cout<<"==================================="<<std::endl;
    std::cout << "Nom : " << infos.at("name") << " Niveau : " << level;
    std::cout << " Rang : " << infos.at("rank");
    if (synthLevel != 0) {std::cout << "+" << synthLevel;}
    std::cout << std::endl << "Type : " << infos.at("type");
    std::cout << " Famille : " << infos.at("family") << std::endl ;
    std::cout << "HP : " << (int) hp <<"/"<< stats.at("hp");
    std::cout << " MP : " << (int) mp <<"/"<<stats.at("mp")<<std::endl;
    std::cout << "Attaque : " << stats.at("atk") << " Defense : " << stats.at("def") << std::endl;
    std::cout << "Agilité : " << stats.at("agi") << " Sagesse : " << stats.at("wis") << std::endl;
    std::cout << "Exp : " << exp << "/" << std::pow(level+1,3) << " ";
    for (int i=0; i<10-(pow(level+1,3)-exp)/(pow(level+1,3)-pow(level,3))*10;i++){std::cout<<"■";}
    for (int i=0; i<(pow(level+1,3)-exp)/(pow(level+1,3)-pow(level,3))*10;i++) { std::cout<<"□";}
    std::cout<<std::endl;
    std::cout<<"==================================="<<std::endl;
}