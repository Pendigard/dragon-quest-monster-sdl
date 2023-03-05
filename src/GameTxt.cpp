#include "GameTxt.h"


void Game::loadGame() {
    std::vector<Monster> mainTeam;
    loadJson("data/database/monster.json",monsterBase);
    loadJson("data/save/save.json",save);
    loadJson("data/database/skill.json",skillBase);
    loadJson("data/database/spell.json",spellBase);
    for (int i = 0; i<save["mainTeam"].Size(); i++)
    {
        rapidjson::Value& monsterData = save["monsters"][save["mainTeam"][i].GetString()];
        Monster m = Monster(monsterData,save["mainTeam"][i].GetString(),monsterBase,skillBase);
        player.mainTeam.push_back(m);
    }
    for (int i = 0; i<save["benchTeam"].Size(); i++)
    {
        rapidjson::Value& monsterData = save["monsters"][save["benchTeam"][i].GetString()];
        Monster m = Monster(monsterData,save["benchTeam"][i].GetString(),monsterBase,skillBase);
        player.benchTeam.push_back(m);
    }
    for (int i = 0; i<save["storageMonsters"].Size(); i++)
    {
        rapidjson::Value& monsterData = save["monsters"][save["storageMonsters"][i].GetString()];
        Monster m = Monster(monsterData,save["storageMonsters"][i].GetString(),monsterBase,skillBase);
        player.storageMonsters.push_back(m);
    }
    player.mainTeam[0].print();
    //player.mainTeam[0].applySkillPoint(40,"Pot de glu",skillBase);
    //player.mainTeam[0].printSpells();
}

void Game::saveGame() {
    for (int i = 0; i<player.mainTeam.size(); i++)
    {
        player.mainTeam[i].updateSaveMonster(save);;
    }
    for (int i = 0; i<player.benchTeam.size(); i++)
    {
        player.benchTeam[i].updateSaveMonster(save);;
    }
    for (int i = 0; i<player.storageMonsters.size(); i++)
    {
        player.storageMonsters[i].updateSaveMonster(save);;
    }
    saveJson("data/save/saveTest.json",save);
}


int main() {
    srand(time(NULL));
    Game g;
    g.loadGame();
    g.player.mainTeam[0].addXp(150000);
    g.player.mainTeam[0].print();
    Monster m = Monster("Pikachu","gluant",g.monsterBase,g.skillBase,g.save);
    g.saveGame();
    return 0;
}