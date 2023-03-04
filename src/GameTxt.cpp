#include "GameTxt.h"


void Game::loadPlayer() {
    std::vector<Monster> mainTeam;
    loadJson("data/database/monster.json",monsterBase);
    loadJson("data/save/save.json",save);
    loadJson("data/database/skill.json",skillBase);
    loadJson("data/database/spell.json",spellBase);
    for (rapidjson::Value::ConstMemberIterator itr = save["mainTeam"].MemberBegin(); itr != save["mainTeam"].MemberEnd(); ++itr)
    {
        rapidjson::Value& monsterData = save["monsters"][itr->value.GetString()];
        Monster m = Monster(monsterData,itr->value.GetString(),monsterBase,skillBase);
        player.mainTeam.push_back(m);
    }
    for (rapidjson::Value::ConstMemberIterator itr = save["benchTeam"].MemberBegin(); itr != save["benchTeam"].MemberEnd(); ++itr)
    {
        rapidjson::Value& monsterData = save["monsters"][itr->value.GetString()];
        Monster m = Monster(monsterData,itr->value.GetString(),monsterBase,skillBase);
        player.benchTeam.push_back(m);
    }
    player.mainTeam[0].print();
    //player.mainTeam[0].applySkillPoint(40,"Pot de glu",skillBase);
    //player.mainTeam[0].printSpells();
}

int main() {
    Game g;
    g.loadPlayer();
    return 0;
}