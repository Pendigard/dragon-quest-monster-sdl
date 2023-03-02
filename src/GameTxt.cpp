#include "GameTxt.h"

void loadJson(std::string path, rapidjson::Document& doc){
    std::ifstream file;
    file.open(path);
    if(file.is_open()){
        std::string line;
        std::string json;
        while(std::getline(file, line)){
            json += line;
        }
        doc.Parse(json.c_str());
    }
}


void Game::loadPlayer() {
    std::vector<Monster> mainTeam;
    loadJson("data/database/monster.json",monsterBase);
    loadJson("data/save/save.json",save);
    for (rapidjson::Value::ConstMemberIterator itr = save["mainTeam"].MemberBegin(); itr != save["mainTeam"].MemberEnd(); ++itr)
    {
        rapidjson::Value& monsterData = save["monsters"][itr->value.GetString()];
        Monster m = Monster(monsterData,itr->value.GetString(),monsterBase);
        player.mainTeam.push_back(m);
    }
    for (rapidjson::Value::ConstMemberIterator itr = save["benchTeam"].MemberBegin(); itr != save["benchTeam"].MemberEnd(); ++itr)
    {
        rapidjson::Value& monsterData = save["monsters"][itr->value.GetString()];
        Monster m = Monster(monsterData,itr->value.GetString(),monsterBase);
        player.benchTeam.push_back(m);
    }
    player.mainTeam[0].afficher();
}

int main() {
    Game g;
    g.loadPlayer();
    return 0;
}