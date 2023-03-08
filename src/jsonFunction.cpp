#include "jsonFunction.h"

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


void loadJson(std::string path, rapidjson::Document& doc) {
    std::ifstream file;
    file.open(path);
    assert(file.is_open());
    std::string line;
    std::string json;
    while(std::getline(file, line)){
        json += line;
    }
    doc.Parse(json.c_str());
}

void saveJson(std::string path, rapidjson::Document& doc) {
    std::ofstream file;
    file.open(path);
    assert(file.is_open());
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    writer.SetMaxDecimalPlaces(1);
    doc.Accept(writer);
    file << buffer.GetString();
}