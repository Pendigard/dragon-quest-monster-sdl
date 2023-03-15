#include "GameTxt.h"

void Game::loadGame()
{
    std::vector<Monster> mainTeam;
    loadJson("data/database/monster.json", monsterBase);
    loadJson("data/save/save.json", save);
    loadJson("data/database/skill.json", skillBase);
    loadJson("data/database/spell.json", spellBase);
    loadJson("data/database/monsterLibrary.json", library);
    for (long unsigned int i = 0; i < save["mainTeam"].Size(); i++)
    {
        rapidjson::Value &monsterData = save["monsters"][save["mainTeam"][i].GetString()];
        Monster m = Monster(monsterData, save["mainTeam"][i].GetString(), monsterBase, skillBase, library);
        player.mainTeam.push_back(m);
    }
    for (long unsigned int i = 0; i < save["benchTeam"].Size(); i++)
    {
        rapidjson::Value &monsterData = save["monsters"][save["benchTeam"][i].GetString()];
        Monster m = Monster(monsterData, save["benchTeam"][i].GetString(), monsterBase, skillBase, library);
        player.benchTeam.push_back(m);
    }
    for (long unsigned int i = 0; i < save["storageMonsters"].Size(); i++)
    {
        rapidjson::Value &monsterData = save["monsters"][save["storageMonsters"][i].GetString()];
        Monster m = Monster(monsterData, save["storageMonsters"][i].GetString(), monsterBase, skillBase, library);
        player.storageMonsters.push_back(m);
    }
}

void Game::saveGame()
{
    for (long unsigned int i = 0; i < player.mainTeam.size(); i++)
    {
        player.mainTeam[i].updateSaveMonster(save);
    }
    for (long unsigned int i = 0; i < player.benchTeam.size(); i++)
    {
        player.benchTeam[i].updateSaveMonster(save);
    }
    for (long unsigned int i = 0; i < player.storageMonsters.size(); i++)
    {
        player.storageMonsters[i].updateSaveMonster(save);
    }
    saveJson("data/save/saveTest.json", save);
}

void Game::deleteFromSave(std::string id)
{
    for (long unsigned int i = 0; i < save["mainTeam"].Size(); i++)
    {
        if (save["mainTeam"][i].GetString() == id)
        {
            save["mainTeam"].Erase(save["mainTeam"].Begin() + i);
            break;
        }
    }
    for (long unsigned int i = 0; i < save["benchTeam"].Size(); i++)
    {
        if (save["benchTeam"][i].GetString() == id)
        {
            save["benchTeam"].Erase(save["benchTeam"].Begin() + i);
            break;
        }
    }
    for (long unsigned int i = 0; i < save["storageMonsters"].Size(); i++)
    {
        if (save["storageMonsters"][i].GetString() == id)
        {
            save["storageMonsters"].Erase(save["storageMonsters"].Begin() + i);
            break;
        }
    }
    if (save["monsters"].HasMember(id.c_str()))
        save["monsters"].RemoveMember(id.c_str());
}

unsigned int Game::askTarget(bool isOffensive, Fight &f)
{
    unsigned int choice;
    std::vector<Monster> targetTeam;
    if (isOffensive)
        targetTeam = f.team2;
    else
        targetTeam = f.team1;
    if (isOffensive)
        std::cout << "Cible : " << std::endl;
    for (long unsigned int i = 0; i < targetTeam.size(); i++)
    {
        if (targetTeam[i].hp > 0)
        {
            std::cout << i + 1 << ". " << targetTeam[i].getName() << "(" << targetTeam[i].getType() << ")" << std::endl;
        }
    }
    std::cout << "Quel monstre voulez-vous cibler ? (1/.../" << targetTeam.size() << ") : ";
    std::cin >> choice;
    return choice;
}

std::vector<std::string> Game::getTargetPlayer(Monster caster, std::string spell, Fight &f)
{
    rapidjson::Value &spellInfo = f.spellBase[spell.c_str()];
    std::vector<std::string> targetTeam;
    if (spellInfo["self"].GetBool())
    {
        targetTeam.push_back(caster.getInfos("id"));
        return targetTeam;
    }
    if (spellInfo["offensive"].GetBool())
    {
        if (spellInfo["multiTarget"].GetBool())
        {
            for (long unsigned int i = 0; i < f.team2.size(); i++)
            {
                targetTeam.push_back(f.team2[i].getInfos("id"));
            }
        }
        else
        {
            targetTeam.push_back(f.team2[askTarget(true, f) - 1].getInfos("id"));
        }
    }
    else
    {
        if (spellInfo["multiTarget"].GetBool())
        {
            for (long unsigned int i = 0; i < f.team1.size(); i++)
            {
                targetTeam.push_back(f.team1[i].getInfos("id"));
            }
        }
        else
        {
            targetTeam.push_back(f.team1[askTarget(false, f) - 1].getInfos("id"));
        }
    }
    return targetTeam;
}

unsigned int Game::orderMonster(Monster m)
{
    std::cout << std::endl
              << m.getName() << " : ";
    std::cout << (int)m.hp << "/" << (int)m.getStat("hp") << "PV";
    std::cout << "|" << (int)m.mp << "/" << (int)m.getStat("mp") << "PM" << std::endl
              << std::endl;
    unsigned int choice;
    std::cout << m.getName() << " : " << std::endl;
    m.printSpells();
    std::cout << "Quel sort voulez-vous utiliser ? (0 pour skip 1/.../" << m.getSpells().size() << "): ";
    std::cin >> choice;
    return choice;
}

void Game::getTacticChoice(Fight &f)
{
    std::vector<std::string> tactics;
    for (long unsigned int i = 0; i < f.team1.size(); i++)
    {
        std::cout << i + 1 << ". " << f.team1[i].getName() << " | " << f.team1[i].getTactic() << std::endl;
    }
    std::cout << "A quel monstre voulez-vous changer de tactique ? (1/.../" << f.team1.size() << "): ";
    unsigned int monsterIndex = 0;
    std::cin >> monsterIndex;
    tactics.push_back("Sans pitié");
    tactics.push_back("Soins avant tout");
    tactics.push_back("Agir avec sagesse");
    tactics.push_back("Pas de magie");
    for (long unsigned int i = 0; i < tactics.size(); i++)
    {
        std::cout << i + 1 << ". " << tactics[i] << std::endl;
    }
    std::cout << "Quelle tactique voulez-vous utiliser ? (1/.../" << tactics.size() << "): ";
    unsigned int choice;
    std::cin >> choice;
    f.team1[monsterIndex].setTactic(tactics[choice - 1]);
}

std::vector<Action> Game::getPlayerChoice(Fight &f)
{
    std::vector<Action> orders;
    unsigned int choice = 0;
    unsigned int choice2 = 0;
    while (choice != 1)
    {
        printOrder(orders, f);
        std::cout << "1. Combattre" << std::endl;
        std::cout << "2. Ordre" << std::endl;
        std::cout << "3. Tactique" << std::endl;
        std::cout << "Que voulez-vous faire ? (1/2/3): ";
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            return orders;
            break;
        case 2:
            orders.clear();
            for (long unsigned int i = 0; i < f.team1.size(); i++)
            {
                if (f.team1[i].hp <= 0)
                    continue;
                choice2 = orderMonster(f.team1[i]);
                if (choice2 != 0)
                {
                    std::vector<std::string> target = getTargetPlayer(f.team1[i], f.team1[i].getSpells()[choice2 - 1], f);
                    orders.push_back(createAction(f.team1[i].getInfos("id"), f.team1[i].getSpells()[choice2 - 1], target));
                }
            }
            break;
        case 3:
            getTacticChoice(f);
            break;
        }
    }
    return orders;
}

void Game::printOrder(std::vector<Action> orders, Fight &f)
{
    std::cout << std::endl;
    for (long unsigned int i = 0; i < orders.size(); i++)
    {
        std::cout << f.getMonsterById(orders[i].idCaster).getName() << " ----- " << orders[i].spell << " -----> ";
        for (int j = 0; j < orders[i].idTargets.size(); j++)
        {
            std::cout << f.getMonsterById(orders[i].idTargets[j]).getName() << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }
}

void Game::fight(Fight &f)
{
    std::vector<Action> orders;
    std::queue<spellImpact> impact;
    std::queue<std::string> messages;
    bool win = false;
    while (f.isOver(win) == false)
    {
        orders = getPlayerChoice(f);
        f.giveActions(orders);
        messages = f.simulateTurn();
        while (!messages.empty())
        {
            std::cout << messages.front() << std::endl;
            messages.pop();
            sleep(1);
        }
        std::cout << std::endl;
        for (long unsigned int i = 0; i < f.team1.size(); i++)
        {
            std::cout << (int)f.team1[i].hp << "/" << (int)f.team1[i].getStat("hp") << "PV | " << (int)f.team1[i].mp << "/" << (int)f.team1[i].getStat("mp") << "PM" << std::endl;
        }
    }
}

std::vector<Monster> Game::createWildMonsterTeam(std::vector<std::string> monsters, unsigned int levelMin, unsigned int levelMax, std::string monster = "null")
{
    std::vector<Monster> team;
    std::string letterId = "A";
    unsigned int teamSize = rand() % 3 + 1;
    std::string name;
    unsigned int level;
    if (monster != "null")
    {
        name = monster + " " + letterId;
        level = rand() % (levelMax - levelMin) + levelMin;
        Monster m = Monster(name, monster, monsterBase, skillBase, save, library, level);
        team.push_back(m);
        letterId[0]++;
        teamSize--;
    }
    for (unsigned int i = 0; i < teamSize; i++)
    {
        std::string randomMonster = monsters[rand() % monsters.size()];
        level = rand() % (levelMax - levelMin) + levelMin;
        name = randomMonster + " " + letterId;
        Monster m = Monster(name, randomMonster, monsterBase, skillBase, save, library, level);
        m.applySkillPoint(5, "Pot de glu", skillBase);
        team.push_back(m);
        letterId[0]++;
    }
    return team;
}

int main()
{
    srand(time(NULL));
    Game g;
    g.loadGame();
    std::vector<std::string> monsters;
    //monsters.push_back("komodor");
    monsters.push_back("gluant");
    //monsters.push_back("jaunyve");
    monsters.push_back("gluanbulle");
    std::vector<Monster> team2 = g.createWildMonsterTeam(monsters, 5, 9);
    std::vector<Monster> team1 = g.player.mainTeam;
    for (long unsigned int i = 0; i < team2.size(); i++)
    {
        team2[i].print();
    }
    g.player.mainTeam[0].addSkillPoint(75);
    g.player.mainTeam[0].applySkillPoint(75, "Pot de glu", g.skillBase);
    // g.saveGame();
    // g.player.mainTeam[1].addXp(1000000);
    // team1 = g.player.mainTeam;
    for (long unsigned int i = 0; i < team1.size(); i++)
    {
        team1[i].print();
    }
    Fight f(team1, team2);
    g.fight(f);
    return 0;
}