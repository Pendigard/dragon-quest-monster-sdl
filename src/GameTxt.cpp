#include "GameTxt.h"


void Game::loadGame() {
    std::vector<Monster> mainTeam;
    loadJson("data/database/monster.json",monsterBase);
    loadJson("data/save/save.json",save);
    loadJson("data/database/skill.json",skillBase);
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

unsigned int Game::askTarget(bool isOffensive,Fight& f) {
    unsigned int choice;
    std::vector<Monster> targetTeam;
    if (isOffensive) targetTeam = f.team2;
    else targetTeam = f.team1;
    if (isOffensive) 
    std::cout<<"Cible : "<<std::endl;
    for (int i = 0; i < targetTeam.size(); i++)
    {
        std::cout << i + 1 << ". " << targetTeam[i].getName() << "(" << targetTeam[i].getType() << ")" << std::endl;
    }
    std::cout<<"Quel monstre voulez-vous cibler ? (1/.../"<<targetTeam.size()<<") : ";
    std::cin >> choice;
    return choice;
}

std::vector<Monster> Game::getTargetPlayer(Monster caster, std::string spell,Fight& f)
{
    rapidjson::Value &spellInfo = f.spellBase[spell.c_str()];
    std::vector<Monster> targetTeam;
    if (spellInfo["self"].GetBool()) {
        targetTeam.push_back(caster);
        return targetTeam;
        }
    if (spellInfo["offensive"].GetBool())
    {
        if (spellInfo["multiTarget"].GetBool()) targetTeam = f.team2;
        else {
            targetTeam.push_back(f.team2[askTarget(true,f)-1]);
        }
    }
    else
    {
        if (spellInfo["multiTarget"].GetBool()) targetTeam = f.team1;
        else {
            targetTeam.push_back(f.team1[askTarget(false,f)-1]);
        }
    }
    return targetTeam;
}

unsigned int Game::orderMonster(Monster m)
{
    unsigned int choice;
    std::cout << m.getName() << " : "<<std::endl;
    m.printSpells();
    std::cout << "Quel sort voulez-vous utiliser ? (0 pour skip 1/.../" << m.getSpells().size() << "): ";
    std::cin >> choice;
    return choice;
}

std::vector<Action> Game::getPlayerChoice(Fight& f)
{
    std::vector<Action> orders;
    unsigned int choice = 0;    
    unsigned int choice2 = 0;
    while (choice != 1)
    {
        printOrder(orders,f);
        std::cout << "1. Combattre" << std::endl;
        std::cout << "2. Ordre" << std::endl;
        std::cout << "3. Tactique" << std::endl;
        std::cout << "Que voulez-vous faire ? (1/2/3): ";
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            return orders;
        case 2:
            for (int i = 0; i < f.team1.size(); i++)
            {
                choice2 = orderMonster(f.team1[i]);
                if (choice2 != 0) {
                    std::vector<Monster> target = getTargetPlayer(f.team1[i], f.team1[i].getSpells()[choice2 - 1], f);
                    orders.push_back(createAction(f.team1[i].getId(), f.team1[i].getSpells()[choice2 - 1],target));
                }
            }
        }
    }
    return orders;
}

void Game::printOrder(std::vector<Action> orders, Fight& f)
{   
    std::cout << std::endl;
    for (int i = 0; i < orders.size(); i++)
    {
        std::cout << f.getMonsterById(orders[i].idCaster).getName() << " ----- " << orders[i].spell << " -----> ";
        for (int j = 0; j < orders[i].idTargets.size(); j++)
        {
            std::cout << orders[i].idTargets[j].getName() << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }
}

void Game::fight(Fight& f) {
    std::vector<Action> orders;
    orders = getPlayerChoice(f);
    f.giveActions(orders);
    f.initTurn();
}
 
int main() {
    srand(time(NULL));
    Game g;
    g.loadGame();
    std::vector<Monster> team1;
    std::vector<Monster> team2;
    team1=g.player.mainTeam;
    Monster m = Monster("Gluant 1","gluant",g.monsterBase,g.skillBase,g.save,12);
    Monster m2 = Monster("Gluant 2","gluant",g.monsterBase,g.skillBase,g.save,25);
    team2.push_back(m);
    team2.push_back(m2);
    Fight f(team1,team2);
    g.fight(f);
    return 0;
}