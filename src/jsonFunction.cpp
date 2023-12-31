#include "jsonFunction.h"


void jsonToUnorderedMap(rapidjson::Value &json, std::unordered_map<std::string, float> &map)
{
    // Boucle permettant de parcourir le json clé après clé
    for (rapidjson::Value::ConstMemberIterator itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr)
    {
        map[itr->name.GetString()] = itr->value.GetFloat();
    }
}

void jsonToUnorderedMap(rapidjson::Value &json, std::unordered_map<std::string, unsigned int> &map)
{
    // Boucle permettant de parcourir le json clé après clé
    for (rapidjson::Value::ConstMemberIterator itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr)
    {
        map[itr->name.GetString()] = itr->value.GetInt();
    }
}

void jsonToVector(rapidjson::Value &jsonList, std::vector<std::string> &vec)
{
    // Boucle permettant de parcourir le json clé après clé
    for (int i = 0; i < jsonList.Size(); i++)
    {
        vec.push_back(jsonList[i].GetString());
    }
}

void unorderedMapToJson(rapidjson::Document &doc, std::unordered_map<std::string, float> &map)
{
    rapidjson::Value json(rapidjson::kObjectType);
    for (auto &x : map)
    {
        rapidjson::Value key(x.first.c_str(), doc.GetAllocator());
        rapidjson::Value value(x.second);
        json.AddMember(key, value, doc.GetAllocator());
    }
    doc.CopyFrom(json, doc.GetAllocator());
}

void loadJson(std::string path, rapidjson::Document &doc)
{
    std::ifstream file;
    file.open(path);
    assert(file.is_open());
    std::string line;
    std::string json;
    while (std::getline(file, line))
    {
        json += line;
    }
    doc.Parse(json.c_str());
}

void saveJson(std::string path, rapidjson::Document &doc)
{
    std::ofstream file;
    file.open(path);
    assert(file.is_open());
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    writer.SetMaxDecimalPlaces(1);
    doc.Accept(writer);
    file << buffer.GetString();
}

std::string formatString(const std::string fmt_str, std::vector<std::string> args)
{
    std::string result;
    size_t cur_pos = 0;
    size_t found_pos;
    size_t arg_index = 0;
    while ((found_pos = fmt_str.find("{}", cur_pos)) != std::string::npos && args.size() > arg_index)
    {
        result.append(fmt_str, cur_pos, found_pos - cur_pos);
        result.append(args[arg_index++]);
        cur_pos = found_pos + 2;
    }
    result.append(fmt_str, cur_pos, fmt_str.length() - cur_pos);
    return result;
}

std::string createSpaceString(int size)
{
    std::string result;
    for (int i = 0; i < size; i++)
    {
        result += " ";
    }
    return result;
}

int getRand(int min, int max)
{
    return rand() % (max - min) + min;
}

std::vector<std::string> splitString(std::string str, const char delimiter[2])
{
    std::vector<std::string> res;
    std::string tmp;
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] == delimiter[0])
        {
            res.push_back(tmp);
            tmp = "";
        }
        else
        {
            tmp += str[i];
        }
    }
    res.push_back(tmp);
    return res;
}

std::string replaceString(std::string str, std::string toReplace, std::string replaceBy) {
    std::string res;
    for (size_t i = 0; i< str.size(); i++) {
        if (str[i] == toReplace[0]) {
            res += replaceBy;
        }
        else {
            res += str[i];
        }
    }   
    return res;
}

std::string maxLenString(std::string str, int maxLen)
{
    if (str.size() > maxLen)
    {
        return str.substr(0, maxLen - 3) + "...";
    }
    return str;
}