#include "GameTxt.h"

void Game::loadGame()
{
    loadJson("data/save/save.json", save);
    database.loadDatabase();
    for (long unsigned int i = 0; i < save["mainTeam"].Size(); i++)
    {
        rapidjson::Value &monsterData = save["monsters"][save["mainTeam"][i].GetString()];
        Monster m = Monster(monsterData, save["mainTeam"][i].GetString(), database);
        player.mainTeam.push_back(m);
    }
    for (long unsigned int i = 0; i < save["benchTeam"].Size(); i++)
    {
        rapidjson::Value &monsterData = save["monsters"][save["benchTeam"][i].GetString()];
        Monster m = Monster(monsterData, save["benchTeam"][i].GetString(), database);
        player.benchTeam.push_back(m);
    }
    for (long unsigned int i = 0; i < save["storageMonsters"].Size(); i++)
    {
        rapidjson::Value &monsterData = save["monsters"][save["storageMonsters"][i].GetString()];
        Monster m = Monster(monsterData, save["storageMonsters"][i].GetString(), database);
        player.storageMonsters.push_back(m);
    }
}

void Game::saveGame()
{
    rapidjson::Value idMain(rapidjson::kArrayType);
    rapidjson::Value idBench(rapidjson::kArrayType);
    rapidjson::Value idStorage(rapidjson::kArrayType);
    for (long unsigned int i = 0; i < player.mainTeam.size(); i++)
    {
        player.mainTeam[i].updateSaveMonster(save);
        rapidjson::Value v(player.mainTeam[i].getInfos("id").c_str(), save.GetAllocator());
        idMain.PushBack(v, save.GetAllocator());
    }
    for (long unsigned int i = 0; i < player.benchTeam.size(); i++)
    {
        player.benchTeam[i].updateSaveMonster(save);
        rapidjson::Value v(player.benchTeam[i].getInfos("id").c_str(), save.GetAllocator());
        idBench.PushBack(v, save.GetAllocator());
    }
    for (long unsigned int i = 0; i < player.storageMonsters.size(); i++)
    {
        player.storageMonsters[i].updateSaveMonster(save);
        rapidjson::Value v(player.storageMonsters[i].getInfos("id").c_str(), save.GetAllocator());
        idStorage.PushBack(v, save.GetAllocator());
    }
    save["mainTeam"] = idMain;
    save["benchTeam"] = idBench;
    save["storageMonsters"] = idStorage;
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
    std::vector<std::string> targetTeam;
    if (isOffensive)
        targetTeam = f.getTeamAlive(f.team2);
    else
        targetTeam = f.getTeamAlive(f.team1);
    if (isOffensive)
        std::cout << "Cible : " << std::endl;
    for (long unsigned int i = 0; i < targetTeam.size(); i++)
    {
        Monster m = f.getMonsterById(targetTeam[i]);
        std::cout << i + 1 << ". " << m.getName() << "(" << m.getType() << ")" << std::endl;
    }
    std::cout << "Quel monstre voulez-vous cibler ? (1/.../" << targetTeam.size() << ") : ";
    std::cin >> choice;
    return choice;
}

std::vector<std::string> Game::getTargetPlayer(Monster caster, std::string spell, Fight &f)
{
    rapidjson::Value &spellInfo = database.getSpellBase()[spell.c_str()];
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
    tactics.push_back("Sans pitié");
    tactics.push_back("Soins avant tout");
    tactics.push_back("Agir avec sagesse");
    tactics.push_back("Pas de magie");
    for (long unsigned int i = 0; i < f.team1.size(); i++)
    {
        std::cout << i + 1 << ". " << f.team1[i].getName() << " | " << tactics[f.team1[i].getTactic()] << std::endl;
    }
    std::cout << "A quel monstre voulez-vous changer de tactique ? (1/.../" << f.team1.size() << "): ";
    unsigned int monsterIndex = 0;
    std::cin >> monsterIndex;
    for (long unsigned int i = 0; i < tactics.size(); i++)
    {
        std::cout << i + 1 << ". " << tactics[i] << std::endl;
    }
    std::cout << "Quelle tactique voulez-vous utiliser ? (1/.../" << tactics.size() << "): ";
    int choice;
    std::cin >> choice;
    Tactics t = (Tactics)(choice - 1);
    f.team1[monsterIndex].setTactic(t);
}

void drawMonstersFight(Fight &f)
{
    std::cout << "Equipe Adverse : " << std::endl;
    for (long unsigned int i = 0; i < f.team2.size(); i++)
    {
        std::cout << std::endl
                  << f.team2[i].getName();
        if (f.team2[i].hp <= 0)
            std::cout << " K.O";
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Equipe 1 : " << std::endl;
    for (long unsigned int i = 0; i < f.team1.size(); i++)
    {
        std::cout << std::endl
                  << f.team1[i].getName() << " (" << f.team1[i].getType() << ") : ";
        if (f.team1[i].hp <= 0)
            std::cout << "K.O";
        else
            std::cout << (int)f.team1[i].hp << "/" << (int)f.team1[i].getStat("hp") << "PV";
        std::cout << "|" << (int)f.team1[i].mp << "/" << (int)f.team1[i].getStat("mp") << "PM" << std::endl;
    }
}

std::vector<Action> Game::getPlayerChoice(Fight &f)
{
    f.scouting = false;
    std::vector<Action> orders;
    unsigned int choice = 0;
    unsigned int choice2 = 0;
    while (choice != 1)
    {
        drawMonstersFight(f);
        printOrder(orders, f);
        std::cout << "1. Combattre" << std::endl;
        std::cout << "2. Ordre" << std::endl;
        std::cout << "3. Tactique" << std::endl;
        std::cout << "4. Dresser" << std::endl;
        std::cout << "5. Fuir" << std::endl;
        std::cout << "Que voulez-vous faire ? (1/.../5): ";
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
        case 4:
            if (f.canScout)
            {
                std::vector<std::string> emptyTarget;
                f.scouting = true;
                for (long unsigned int i = 0; i < f.team1.size(); i++)
                {
                    orders.push_back(createAction(f.team1[i].getInfos("id"), "null", emptyTarget));
                }
                return orders;
            }
            break;
        case 5:
            if (f.flee())
            {
                std::cout << "Vous prenez la fuite." << std::endl;
                sleep(1);
                f.teamFlee = true;
                return orders;
            }
            else
            {
                std::cout << "Vous prenez la fuite." << std::endl;
                sleep(1);
                std::cout << "Mais les monstres vous bloquent le chemin." << std::endl;
                orders.empty();
                std::vector<std::string> emptyTarget;
                for (long unsigned int i = 0; i < f.team1.size(); i++)
                {
                    orders.push_back(createAction(f.team1[i].getInfos("id"), "null", emptyTarget));
                }
                return orders;
            }
        default:
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
        Monster m = Monster(name, monster, database, save, level);
        team.push_back(m);
        letterId[0]++;
        teamSize--;
    }
    for (unsigned int i = 0; i < teamSize; i++)
    {
        std::string randomMonster = monsters[rand() % monsters.size()];
        level = rand() % (levelMax - levelMin) + levelMin;
        name = randomMonster + " " + letterId;
        Monster m = Monster(name, randomMonster, database, save, level);
        m.autoAttributeSkill(database);
        team.push_back(m);
        letterId[0]++;
    }
    return team;
}

void Game::scoutMonster(std::string id, Fight &f)
{
    std::string name;
    std::cout << "Donnez un nom au monstre:";
    std::cin >> name;
    Monster monster = f.getMonsterById(id);
    monster.setName(name);
    std::cout << std::endl
              << "Voulez-vous l'ajouter à votre équipe ? (y/n):";
    std::string answer;
    std::cin >> answer;
    monster.createSaveMonster(save);
    switch (answer[0])
    {
    case 'y':
        if (player.mainTeam.size() == 3)
        {
            std::cout << std::endl
                      << "Votre équipe est déjà pleine:" << std::endl;
            for (long unsigned int i = 0; i < player.mainTeam.size(); i++)
            {
                std::cout << i + 1 << ". " << player.mainTeam[i].getName() << " (" << player.mainTeam[i].getInfos("type") << ")" << std::endl;
            }
            std::cout << "Quel monstre voulez-vous remplacer ?(0 pour l'envoyer en réserve/1/2/3):";
            int choice;
            std::cin >> choice;
            if (choice == 0)
            {
                player.storageMonsters.push_back(monster);
                break;
            }
            else
            {
                player.storageMonsters.push_back(player.mainTeam[choice - 1]);
                player.mainTeam[choice - 1] = monster;
                break;
            }
        }
        else
        {
            player.mainTeam.push_back(monster);
            break;
        }
    case 'n':
        player.storageMonsters.push_back(monster);
        break;
    }
}

bool Game::fight(Fight &f)
{
    std::vector<Action> orders;
    std::queue<spellImpact> impact;
    std::queue<std::string> messages;
    bool win = false;
    while (f.isOver(win) == false)
    {
        orders = getPlayerChoice(f);
        if (f.teamFlee)
        {
            player.mainTeam = f.team1;
            return true;
        }
        if (f.scouting)
        {
            std::string monsterToScout = getTargetPlayer(f.team1[0], "Dressage", f)[0];
            bool scouted = f.scout(monsterToScout, messages);
            while (!messages.empty())
            {
                std::cout << messages.front() << std::endl
                          << std::endl;
                messages.pop();
                sleep(1);
            }
            if (scouted)
            {
                player.mainTeam = f.team1;
                scoutMonster(monsterToScout, f);
                return true;
            }
        }
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
    if (win)
    {
        getReward(f);
        return true;
    }
    else
    {
        sleep(1);
        std::cout << "Votre équipe est décimé" << std::endl;
        player.removeGold(player.getGold() / 2);
        return false;
    }
}

void Game::attributePoint(Monster &m)
{
    bool end = false;
    while (!end && m.getSkillToAttribute() > 0)
    {
        int i = 0;
        std::vector<std::string> skills;
        std::cout << std::endl;
        std::cout << "Vous avez " << m.getSkillToAttribute() << " points à répartir à " << m.getName() << std::endl;
        std::cout << std::endl;
        sleep(1);
        for (auto &skill : m.getSkills())
        {
            std::cout << i + 1 << ". " << skill.first << " : " << skill.second << std::endl;
            skills.push_back(skill.first);
            i++;
        }
        std::cout << "Quelle compétence voulez-vous améliorer ? (0 pour quitter 1/" << skills.size() << "):";
        int choice;
        std::cin >> choice;
        if (choice == 0)
        {
            end = true;
        }
        else
        {
            std::cout << "De combien voulez-vous l'améliorer ? (0 pour quitter):";
            int value;
            std::cin >> value;
            if (value == 0)
            {
                end = true;
            }
            else
            {
                int indexSpellAlreadyLearned = m.getSpells().size();
                m.applySkillPoint(value, skills[choice - 1], database);
                for (long unsigned int j = indexSpellAlreadyLearned; j < m.getSpells().size(); j++)
                {
                    std::cout << m.getName() << " apprend le sort " << m.getSpells()[j] << std::endl;
                }
            }
        }
    }
}

void Game::getReward(Fight &f)
{
    sleep(1);
    std::cout << "Vous remportez le combat" << std::endl;
    player.mainTeam = f.team1;
    sleep(1);
    int gold = f.getGoldReward(database);
    player.addGold(gold);
    std::cout << "Vous gagnez " << gold << " pièces d'or" << std::endl;
    sleep(1);
    int exp = f.getExpReward(database);
    std::cout << "Vos monstres gagnent " << exp << " points d'expérience" << std::endl;
    sleep(1);
    for (long unsigned int i = 0; i < player.mainTeam.size(); i++)
    {
        bool hasLevelUp = player.mainTeam[i].addXp(exp);
        if (hasLevelUp)
        {
            std::cout << player.mainTeam[i].getName() << " passe au niveau " << player.mainTeam[i].getLevel() << std::endl;
            sleep(1);
            if (player.mainTeam[i].getSkillToAttribute() > 0)
            {
                attributePoint(player.mainTeam[i]);
            }
        }
    }
}

void Game::healTeam()
{
    for (long unsigned int i = 0; i < player.mainTeam.size(); i++)
    {
        player.mainTeam[i].fullHeal();
    }
}

void Game::exploration(std::vector<std::string> monsterZone, int niveauMin, int niveauMax)
{
    bool alive = true;
    do
    {
        std::vector<Monster> zone1Team = createWildMonsterTeam(monsterZone, niveauMin, niveauMax);
        Fight f(player.mainTeam, zone1Team);
        std::cout << "Un groupe de monstre vous attaque" << std::endl;
        sleep(1);
        alive = fight(f);
        if (!alive)
        {
            std::cout << "Une équipe de secours vous vient en aide" << std::endl;
            sleep(1);
            std::cout << "Vous perdez la moitié de votre or..." << std::endl;
            sleep(1);
            std::cout << "Votre équipe est soignée" << std::endl;
            sleep(1);
            healTeam();
            return;
        }
        std::cout << "1. Continuer l'exploration" << std::endl;
        std::cout << "2. Partir" << std::endl;
        std::cout << "Que voulez-vous faire ?(1/2):";
        int choice;
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            break;
        case 2:
            std::cout << "Vous partez de la zone..." << std::endl;
            sleep(1);
            return;
        }
    } while (true);
}

void Game::play()
{
    std::vector<std::string> zone1;
    zone1.push_back("komodor");
    zone1.push_back("gluant");
    zone1.push_back("medhigluant");
    std::vector<std::string> zone2;
    bool end = false;
    while (!end)
    {
        std::cout << "1. Explorer les zones sauvages" << std::endl;
        std::cout << "2. Aller au centre des dresseurs" << std::endl;
        std::cout << "3. Quitter" << std::endl;
        std::cout << "Que voulez-vous faire ?(1/2/3):";
        int choice;
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            std::cout << "1. La zone 1" << std::endl;
            std::cout << "Quelle zone voulez-vous explorer ?(1):";
            int zone;
            std::cin >> zone;
            switch (zone)
            {
            case 1:
                exploration(zone1, 1, 5);
                break;
            }
            break;
        case 2:
            scoutCenter();
            break;
        case 3:
            end = true;
            break;
        }
    }
}

void Game::organizeMonsters()
{
    std::cout << "1. Envoyer un monstre en réserve" << std::endl;
    std::cout << "2. Ajouter un monstre à l'équipe" << std::endl;
    std::cout << "Que voulez-vous faire(1/2):";
    int choice;
    std::cin >> choice;
    switch (choice)
    {
    case 1:
        if (player.mainTeam.size() == 1)
        {
            std::cout << "Votre équipe ne peut pas être vide" << std::endl;
            sleep(1);
            break;
        }
        for (long unsigned int i = 0; i < player.mainTeam.size(); i++)
        {
            std::cout << i + 1 << ": " << std::endl;
            player.mainTeam[i].print();
            std::cout << std::endl;
        }
        std::cout << "Quel monstre voulez-vous envoyer en réserve ?(1/2/3):";
        int choice2;
        std::cin >> choice2;
        player.addMonster(player.mainTeam[choice2 - 1], 3);
        player.removeMonster(choice2 - 1, 1);
        std::cout << "Monstre envoyé en réserve" << std::endl
                  << std::endl;
        sleep(1);
        break;
    case 2:
        if (player.storageMonsters.size() == 0)
        {
            std::cout << "Votre réserve est vide" << std::endl
                      << std::endl;
            sleep(1);
            break;
        }
        if (player.mainTeam.size() == 3)
        {
            std::cout << "Votre équipe est pleine" << std::endl
                      << std::endl;
            sleep(1);
            break;
        }
        for (long unsigned int i = 0; i < player.storageMonsters.size(); i++)
        {
            std::cout << i + 1 << ". " << player.storageMonsters[i].getName() << " (" << player.storageMonsters[i].getInfos("type") << ")" << std::endl;
        }
        std::cout << "Quel monstre voulez-vous ajouter à l'équipe ?(1/2/3):";
        int choice3;
        std::cin >> choice3;
        player.addMonster(player.storageMonsters[choice3 - 1], 1);
        player.removeMonster(choice3 - 1, 3);
        std::cout << "Vous avez ajouté " << player.mainTeam[player.mainTeam.size() - 1].getName() << " à votre équipe" << std::endl
                  << std::endl;
        sleep(1);
        break;
    }
}

void Game::deleteMonster(std::string id)
{
    for (long unsigned int i = 0; i < player.storageMonsters.size(); i++)
    {
        if (player.storageMonsters[i].getInfos("id") == id)
        {
            player.storageMonsters.erase(player.storageMonsters.begin() + i);
            deleteFromSave(id);
            return;
        }
    }
    for (long unsigned int i = 0; i < player.mainTeam.size(); i++)
    {
        if (player.mainTeam[i].getInfos("id") == id)
        {
            player.mainTeam.erase(player.mainTeam.begin() + i);
            deleteFromSave(id);
            return;
        }
    }
    for (long unsigned int i = 0; i < player.benchTeam.size(); i++)
    {
        if (player.benchTeam[i].getInfos("id") == id)
        {
            player.benchTeam.erase(player.benchTeam.begin() + i);
            deleteFromSave(id);
            return;
        }
    }
}

std::vector<Monster> Game::getSynthetisableMonster(std::string charge, std::string firstChoice="null")
{
    std::vector<Monster> synthetisableMonsters;
    for (long unsigned int i = 0; i < player.storageMonsters.size(); i++)
    {
        if ((player.storageMonsters[i].getInfos("charge") == charge || player.storageMonsters[i].getInfos("charge") == "neutre")  && player.storageMonsters[i].getLevel() >= 10 && player.storageMonsters[i].getInfos("id") != firstChoice)
        {
            synthetisableMonsters.push_back(player.storageMonsters[i]);
        }
    }
    return synthetisableMonsters;
}

std::unordered_map<std::string, unsigned int> Game::getSkillSynthChoice(Monster m1, Monster m2, std::string child)
{
    std::unordered_map<std::string, unsigned int> possibleSkill = getSynthesisSkills(m1, m2, child, database);
    std::unordered_map<std::string, unsigned int> chosenSkill;
    bool end = false;
    int setChosen = 0;
    while (!end && setChosen < possibleSkill.size() && setChosen <= 3)
    {
        std::vector<std::string> skillName;
        int i = 0;
        for (auto it = possibleSkill.begin(); it != possibleSkill.end(); it++)
        {
            std::cout << i + 1 << ". " << it->first << ":" << it->second << "pts" << std::endl;
            skillName.push_back(it->first);
            i++;
        }
        std::cout << "Choisissez un set de compétence à ajouter au monstre(1/" << i + 1 << "):";
        int choice;
        std::cin >> choice;
        choice--;
        std::cout << skillName[choice] << std::endl;
        chosenSkill[skillName[choice]] = possibleSkill[skillName[choice]];
        setChosen++;
        possibleSkill.erase(skillName[choice]);
    }
    return chosenSkill;
}

void Game::synthesis()
{
    std::vector<Monster> synthetisableMonstersPlus = getSynthetisableMonster("plus");
    if (synthetisableMonstersPlus.size() == 0)
    {
        std::cout << "Vous n'avez pas de monstre de charge plus à synthétiser" << std::endl;
        sleep(1);
        return;
    }
    for (long unsigned int i = 0; i < synthetisableMonstersPlus.size(); i++)
    {
        std::cout << i + 1 << ":" << std::endl;
        synthetisableMonstersPlus[i].print();
    }
    std::cout << "Choisissez un monstre de charge plus à synthétiser(0 pour annuler /1/.../" << synthetisableMonstersPlus.size() << "):";
    int monsterPlus;
    std::cin >> monsterPlus;
    if (monsterPlus == 0)
        return;
    std::vector<Monster> synthetisableMonstersMoins = getSynthetisableMonster("moins",synthetisableMonstersPlus[monsterPlus - 1].getInfos("id"));
    if (synthetisableMonstersMoins.size() == 0)
    {
        std::cout << "Vous n'avez pas de monstre de charge moins à synthétiser" << std::endl;
        sleep(1);
        return;
    }
    for (long unsigned int i = 0; i < synthetisableMonstersMoins.size(); i++)
    {
        std::cout << i + 1 << ":" << std::endl;
        synthetisableMonstersMoins[i].print();
    }
    std::cout << "Choisissez un monstre de charge moins à synthétiser(0 pour annuler /1/.../" << synthetisableMonstersMoins.size() << "):";
    int monsterMoins;
    std::cin >> monsterMoins;
    if (monsterMoins == 0)
        return;
    std::cout << "Vous synthétisez:" << std::endl;
    synthetisableMonstersPlus[monsterPlus - 1].print();
    std::cout << std::endl
              << "X" << std::endl
              << std::endl;
    synthetisableMonstersMoins[monsterMoins - 1].print();
    std::cout << std::endl;
    std::vector<std::string> childrenMonsters = makeSynthesisPreview(synthetisableMonstersPlus[monsterPlus - 1], synthetisableMonstersMoins[monsterMoins - 1],database);
    for (long unsigned int i = 0; i < childrenMonsters.size(); i++)
    {
        std::cout << i + 1 << ". " << childrenMonsters[i] << " Rang:" << database.getLibrary()[childrenMonsters[i].c_str()]["rank"].GetString();
        std::cout << " Famille:" << database.getLibrary()[childrenMonsters[i].c_str()]["family"].GetString() << std::endl;
    }
    std::cout << "Choisissez le monstre résultat (0 pour annuler /1/.../" << childrenMonsters.size() << "):";
    int child;
    std::cin >> child;
    if (child == 0)
        return;
    std::unordered_map<std::string, unsigned int> chosenSkill = getSkillSynthChoice(synthetisableMonstersPlus[monsterPlus - 1], synthetisableMonstersMoins[monsterMoins - 1], childrenMonsters[child - 1]);
    std::string name;
    std::cout << "Choisissez un nom pour votre monstre:";
    std::cin >> name;
    std::cout << "Synthétisation en cours..." << std::endl;
    sleep(1);
    Monster childMonster = Monster(synthetisableMonstersPlus[monsterPlus - 1], synthetisableMonstersMoins[monsterMoins - 1], name, childrenMonsters[child - 1], database, chosenSkill, save);
    deleteMonster(synthetisableMonstersPlus[monsterPlus - 1].getInfos("id"));
    deleteMonster(synthetisableMonstersMoins[monsterMoins - 1].getInfos("id"));
    childMonster.createSaveMonster(save);
    player.addMonster(childMonster, 3);
    std::cout << "Synthétisation terminée !" << std::endl;
    sleep(1);
    std::cout << "Vous avez obtenu:" << std::endl;
    childMonster.print();
    std::cout << "Appuyez sur entrée pour continuer" << std::endl;
    std::cin.ignore();
    std::cout << "Le monstre est envoyé dans la réserve" << std::endl;
    sleep(1);
}

void Game::scoutCenter()
{
    bool out = false;
    while (!out)
    {
        std::cout << "Bienvenue au centre des dresseurs !" << std::endl;
        std::cout << "1. Soigner votre équipe" << std::endl;
        std::cout << "2. Faire une synthèse de monstre" << std::endl;
        std::cout << "3. Organiser votre équipe" << std::endl;
        std::cout << "4. Sauvegarder" << std::endl;
        std::cout << "5. Partir" << std::endl;
        std::cout << "Que voulez-vous faire ?(1/.../5):";
        int choice;
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            healTeam();
            std::cout << "Votre équipe est guérit" << std::endl
                      << std::endl;
            sleep(1);
            break;
        case 2:
            synthesis();
            break;
        case 3:
            organizeMonsters();
            break;
        case 4:
            saveGame();
            std::cout << "Sauvegarde effectuée" << std::endl
                      << std::endl;
            sleep(1);
            break;
        case 5:
            out = true;
            break;
        }
    }
}

int main()
{
    srand(time(NULL));
    Game g;
    g.loadGame();
    g.play();
    return 0;
}