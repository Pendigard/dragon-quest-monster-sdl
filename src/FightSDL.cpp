
#include "FightSDL.h"

FightSDL::FightSDL(Fight &fight, SDL_Renderer *renderer, Sprite *cursor, Database *database)
{
    this->database = database;
    this->fight = fight;
    this->renderer = renderer;
    this->cursor = cursor;
    createTeamSprite(fight.team1);
    createTeamSprite(fight.team2, 2);
    createTeamSpriteIcon(fight.team1);
    this->timeLastFrame = SDL_GetTicks();
    this->camera.x = 0;
    this->camera.y = 0;
    this->caster = -1;
    this->tacticIcons = Sprite(renderer, "data/sprite/tactic_icon/tactic_", 4);
    createMenu();
    currentMenu = "main";
    tacticIndex = nullTactic;
    this->status = mainMenu;
    lastTimeSpellImpact = SDL_GetTicks();
}

FightSDL::~FightSDL()
{
}

void FightSDL::drawTeam(bool team1)
{
    std::vector<Sprite> &team = team1 ? teamSprites1 : teamSprites2;
    std::vector<Monster> &fightTeam = team1 ? fight.team1 : fight.team2;
    float size = 3;
    switch (team.size())
    {
    case 1:
        if (fightTeam[0].getHp() > 0)
            team[0].draw(400, 350, camera, false, size, true);
        break;
    case 2:
        if (fightTeam[0].getHp() > 0)
            team[0].draw(300, 350, camera, false, size, true);
        if (fightTeam[1].getHp() > 0)
            team[1].draw(500, 350, camera, false, size, true);
        break;
    case 3:
        if (fightTeam[0].getHp() > 0)
            team[0].draw(200, 350, camera, false, size, true);
        if (fightTeam[1].getHp() > 0)
            team[1].draw(400, 350, camera, false, size, true);
        if (fightTeam[2].getHp() > 0)
            team[2].draw(600, 350, camera, false, size, true);
        break;
    default:
        break;
    }
}

void FightSDL::createTeamSprite(std::vector<Monster> &team, int teamIndex)
{
    if (teamIndex == 1)
    {
        teamSprites1.clear();
        teamSprites1.resize(team.size());
    }
    else
    {
        teamSprites2.clear();
        teamSprites2.resize(team.size());
    }
    for (size_t i = 0; i < team.size(); i++)
    {
        if (teamIndex == 1)
            teamSprites1[i] = Sprite(renderer, "data/sprite/fighting_sprite/" + team[i].getInfos("type") + ".png", 1);
        else
            teamSprites2[i] = Sprite(renderer, "data/sprite/fighting_sprite/" + team[i].getInfos("type") + ".png", 1);
    }
}

void FightSDL::createTeamSpriteIcon(std::vector<Monster> &team)
{
    teamSpritesIcon1.clear();
    teamSpritesIcon1.resize(team.size());
    for (size_t i = 0; i < team.size(); i++)
    {
        teamSpritesIcon1[i] = Sprite(renderer, "data/sprite/overworld_monsters/" + team[i].getInfos("type") + "/" + team[i].getInfos("type") + "_front_", 3);
    }
}

void FightSDL::drawTeamInfo(std::vector<Monster> &team, std::vector<Sprite> &teamIcons)
{
    int x = 10;
    int y = 10;
    int width = 250;
    int height = 100;
    SDL_Color white = {255, 255, 255};
    for (size_t i = 0; i < team.size(); i++)
    {
        drawBox(renderer, x + i * (260), y, width, height);
        drawText(renderer, "Nom: " + team[i].getInfos("name"), x + i * 260 + 10, y + 10, 12, white);
        teamIcons[i].draw(x + i * (260) + 10, y + 25, camera, false, 2);
        std::string hp = "PV: " + std::to_string(team[i].getHp()) + "/" + std::to_string((int)team[i].getStat("hp"));
        drawText(renderer, hp, x + i * (260) + 80, y + 35, 12, white);
        std::string mp = "PM: " + std::to_string(team[i].getMp()) + "/" + std::to_string((int)team[i].getStat("mp"));
        drawText(renderer, mp, x + i * (260) + 80, y + 55, 12, white);
        Tactics tactic = team[i].getTactic();
        tacticIcons.setCurrentSprite(tactic);
        tacticIcons.draw(x + i * (260) + 218, y + 68, camera, false, 1);
    }
}

std::vector<Option> createSingleOptionRow(Option option)
{
    std::vector<Option> options;
    options.push_back(option);
    return options;
}

void FightSDL::createMenu()
{
    Menu menu(cursor);
    std::vector<Option> options;
    menu.addRow(createSingleOptionRow(createOption("Combattre", 10, 450, 220, 45, RIGHT, simulateFight)));
    menu.addRow(createSingleOptionRow(createOption("Tactique", 10, 500, 220, 45, RIGHT, tacticMenu)));
    menu.addRow(createSingleOptionRow(createOption("Ordre", 10, 550, 220, 45, RIGHT, orderMenu)));
    menu.addRow(createSingleOptionRow(createOption("Dressage", 10, 600, 220, 45, RIGHT, scoutMenu)));
    menu.addRow(createSingleOptionRow(createOption("Fuir", 10, 650, 220, 45, RIGHT)));
    menu.changePageX = false;
    menus["main"] = menu;
    menu.clear();
    for (size_t i = 0; i < teamSprites1.size(); i++)
    {
        options.push_back(createOption("", 10 + i * 260, 10, 250, 100, DOWN, casterIndex, true));
    }
    menu.addRow(options);
    options.clear();
    menus["team"] = menu;
    menu.clear();
    switch (teamSprites2.size())
    {
    case 1:
        options.push_back(createOption("", 400, 350 - teamSprites2[0].getHeight(), teamSprites2[0].getWidth() / 2, 100, UP, targetIndex, true));
        break;
    case 2:
        options.push_back(createOption("", 300, 350 - teamSprites2[0].getHeight(), teamSprites2[0].getWidth() / 2, 100, UP, targetIndex, true));
        options.push_back(createOption("", 500, 350 - teamSprites2[1].getHeight(), teamSprites2[1].getWidth() / 2, 100, UP, targetIndex, true));
        break;
    case 3:
        options.push_back(createOption("", 200, 350 - teamSprites2[0].getHeight(), teamSprites2[0].getWidth() / 2, 100, UP, targetIndex, true));
        options.push_back(createOption("", 400, 350 - teamSprites2[1].getHeight(), teamSprites2[1].getWidth() / 2, 100, UP, targetIndex, true));
        options.push_back(createOption("", 600, 350 - teamSprites2[2].getHeight(), teamSprites2[2].getWidth() / 2, 100, UP, targetIndex, true));
        break;
    }
    menu.addRow(options);
    options.clear();
    menus["enemy"] = menu;
    for (size_t i = 0; i < fight.team1.size(); i++)
    {
        menu.clear();
        int page = 0;
        size_t spell = 0;
        std::vector<std::string> spells = fight.team1[i].getSpells();
        while (spell < spells.size())
        {
            size_t tmp = spell;
            for (size_t j = 0; j < std::min(spells.size() - tmp, (size_t)5); j++)
            {
                std::string label = spells[j + tmp];
                rapidjson::Document &spellBase = database->getSpellBase();
                assert(spellBase.HasMember(spells[j + tmp].c_str()));
                if (spellBase[spells[j + tmp].c_str()].HasMember("manaCost") && spellBase[spells[j + tmp].c_str()]["manaCost"].GetInt() > 0)
                {
                    label = label + createSpaceString(15 - label.size()) + " PM: " + std::to_string(spellBase[spells[j + tmp].c_str()]["manaCost"].GetInt());
                }
                menu.addRow(createSingleOptionRow(createOption(label, 10, 450 + j * 50, 550, 45, RIGHT, spellIndex)), page);
                spell++;
            }
            page++;
        }
        menus["spell" + std::to_string(i)] = menu;
    }
    menu.clear();
    menu.addRow(createSingleOptionRow(createOption("Sans pitie", 20, 450, 370, 45, RIGHT)));
    menu.addRow(createSingleOptionRow(createOption("Soins avant tout", 20, 500, 370, 45, RIGHT)));
    menu.addRow(createSingleOptionRow(createOption("Agir avec sagesse", 20, 550, 370, 45, RIGHT)));
    menu.addRow(createSingleOptionRow(createOption("Pas de magie", 20, 600, 370, 45, RIGHT)));
    menus["tactic"] = menu;
}

void FightSDL::updateMonsterMenu()
{
    for (size_t i = 0; i < fight.team1.size(); i++)
    {
        menus["team"].setForbiddenChoice(i, 0, fight.team1[i].getHp() <= 0);
        menus["team"].setFirstChoice();
    }
    for (size_t i = 0; i < fight.team2.size(); i++)
    {
        menus["enemy"].setForbiddenChoice(i, 0, fight.team2[i].getHp() <= 0);
        menus["enemy"].setFirstChoice();
    }
}

void FightSDL::changeCurrentMenu(std::string menu)
{
    previousMenu.push(currentMenu);
    currentMenu = menu;
}

void FightSDL::returnToPreviousMenu()
{
    if (previousMenu.empty())
    {
        caster = -1;
        this->status = mainMenu;
        pointToAlly = true;
        stepStatus = 0;
        return;
    }

    currentMenu = previousMenu.top();
    previousMenu.pop();
    stepStatus--;
}

void FightSDL::addOrder(Action action)
{
    for (size_t i = 0; i < orders.size(); i++)
    {
        if (orders[i].idCaster == action.idCaster)
        {
            orders[i] = action;
            return;
        }
    }
    orders.push_back(action);
}

void FightSDL::removeOrder(std::string idCaster)
{
    for (size_t i = 0; i < orders.size(); i++)
    {
        if (orders[i].idCaster == idCaster)
        {
            orders.erase(orders.begin() + i);
            return;
        }
    }
}

int FightSDL::getMonsterIndex(std::string idCaster, bool &team)
{
    for (size_t i = 0; i < fight.team1.size(); i++)
    {
        if (fight.team1[i].getInfos("id") == idCaster)
        {
            team = true;
            return i;
        }
    }
    for (size_t i = 0; i < fight.team2.size(); i++)
    {
        if (fight.team2[i].getInfos("id") == idCaster)
        {
            team = false;
            return i;
        }
    }
    return -1;
}

void FightSDL::drawOrder()
{
    SDL_Color white = {255, 255, 255};
    if (orders.size() > 0)
        drawBox(renderer, 10, 150, 600, 5 + 25 * orders.size());
    for (size_t i = 0; i < orders.size(); i++)
    {
        bool teamCaster;
        bool teamTarget;
        std::string name;
        int casterIndex = getMonsterIndex(orders[i].idCaster, teamCaster);
        if (teamCaster)
            name = fight.team1[casterIndex].getName();
        else
            name = fight.team2[casterIndex].getName();
        std::string label = name + " ----" + orders[i].spell;
        int targetIndex = getMonsterIndex(orders[i].idTargets[0], teamTarget);
        if (orders[i].idTargets.size() == 1)
        {
            if (teamTarget)
                label = label + "----> " + fight.team1[targetIndex].getName();
            else
                label = label + "----> " + fight.team2[targetIndex].getName();
        }
        else if (teamTarget == true)
        {
            label = label + "----> les allies";
        }
        else
        {
            label = label + "----> l'equipe adverse";
        }
        drawText(renderer, label, 20, 160 + i * 20, 12, white);
    }
}

bool FightSDL::getPlayerFightChoice()
{
    switch (menus[currentMenu].getChoice())
    {
    case simulateFight:
        stepStatus = 0;
        status = simulateTurn;
        lastTimeSpellImpact = SDL_GetTicks();
        fight.giveActions(orders);
        fight.initTurn();
        orders.clear();
        return false;
    case tacticMenu:
        stepStatus = 0;
        caster = -1;
        status = tactic;
        return false;
    case orderMenu:
        stepStatus = 0;
        caster = -1;
        status = order;
        return false;
    case scoutMenu:
        stepStatus = 0;
        status = scout;
        return false;
    default:
        return false;
    }
}

void FightSDL::checkChoiceSet()
{
    switch (status)
    {
    case tactic:
        if (stepStatus == 2)
        {
            tacticIndex = (Tactics)menus[currentMenu].getCurrentChoiceY();
            fight.team1[caster].setTactic(tacticIndex);
            removeOrder(fight.team1[caster].getInfos("id"));
            caster = -1;
            tacticIndex = nullTactic;
            returnToPreviousMenu();
        }
        else if (stepStatus == 1)
        {
            caster = menus[currentMenu].getCurrentChoiceX();
            changeCurrentMenu("tactic");
            stepStatus++;
        }
        else
        {
            changeCurrentMenu("team");
            stepStatus++;
        }
        break;
    case order:
        if (stepStatus == 3)
        {
            std::string spell = fight.team1[caster].getSpells()[spellChoice];
            std::vector<std::string> targetId;
            if (database->getSpellBase()[spell.c_str()]["offensive"].GetBool())
            {
                targetId.push_back(fight.team2[menus[currentMenu].getCurrentChoiceX()].getInfos("id"));
            }
            else
            {
                targetId.push_back(fight.team1[menus[currentMenu].getCurrentChoiceX()].getInfos("id"));
            }
            addOrder(createAction(fight.team1[caster].getInfos("id"), spell, targetId));
            returnToPreviousMenu();
            returnToPreviousMenu();
        }
        else if (stepStatus == 2)
        {
            spellChoice = menus[currentMenu].getCurrentChoiceY() + menus[currentMenu].getCurrentPage() * 5;
            std::string spell = fight.team1[caster].getSpells()[spellChoice];
            if (!database->getSpellBase()[spell.c_str()]["multiTarget"].GetBool() && !database->getSpellBase()[spell.c_str()]["self"].GetBool())
            {
                if (database->getSpellBase()[spell.c_str()]["offensive"].GetBool())
                {
                    changeCurrentMenu("enemy");
                }
                else
                {
                    changeCurrentMenu("team");
                }
                stepStatus++;
            }
            else
            {
                std::vector<std::string> targetId;
                if (database->getSpellBase()[spell.c_str()]["self"].GetBool())
                {
                    targetId.push_back(fight.team1[caster].getInfos("id"));
                }
                else
                {
                    if (database->getSpellBase()[spell.c_str()]["offensive"].GetBool())
                    {
                        for (size_t i = 0; i < fight.team2.size(); i++)
                        {
                            targetId.push_back(fight.team2[i].getInfos("id"));
                        }
                    }
                    else
                    {
                        for (size_t i = 0; i < fight.team1.size(); i++)
                        {
                            targetId.push_back(fight.team1[i].getInfos("id"));
                        }
                    }
                }
                addOrder(createAction(fight.team1[caster].getInfos("id"), spell, targetId));
                returnToPreviousMenu();
            }
        }
        else if (stepStatus == 1)
        {
            caster = menus[currentMenu].getCurrentChoiceX();
            changeCurrentMenu("spell" + std::to_string(caster));
            stepStatus++;
        }
        else
        {
            changeCurrentMenu("team");
            stepStatus++;
        }
        break;
    case scout:
        if (stepStatus == 1)
        {
            std::vector<std::string> emptyTarget;
            fight.scouting = true;
            for (long unsigned int i = 0; i < fight.team1.size(); i++)
            {
                orders.push_back(createAction(fight.team1[i].getInfos("id"), "null", emptyTarget));
            }
            stepStatus++;
        }
        else
        {
            changeCurrentMenu("enemy");
            stepStatus++;
        }
        break;
    default:
        break;
    }
}

void FightSDL::simulateActions()
{
    if (fight.actionsOrdered.size() > 0)
    {
        if (spellImpacts.size() == 0)
        {
            getMonsterIndex(fight.actionsOrdered.front().idCaster, pointToAlly);
            spellImpacts = fight.simulateAction();
            lastTimeSpellImpact = SDL_GetTicks();
        }
        else
        {
            SDL_Color color = {255, 255, 255};
            drawBox(renderer, 15, 520, 760, 120);
            drawText(renderer, spellImpacts.front().message, 25, 530, 15, color);
            if (lastTimeSpellImpact + 2000 < SDL_GetTicks())
            {
                fight.updateMonster(spellImpacts.front());
                spellImpacts.pop();
                lastTimeSpellImpact = SDL_GetTicks();
                if (spellImpacts.size() > 0)
                {
                    if (spellImpacts.front().targetId.size() > 0)
                        getMonsterIndex(spellImpacts.front().targetId.front(), pointToAlly);
                }
                else
                {
                    pointToAlly = false;
                }
            }
        }
    }
    else
    {
        updateMonsterMenu();
        pointToAlly = true;
        status = mainMenu;
    }
}

bool FightSDL::runFight()
{
    bool quit = false;
    SDL_Event event;
    Sprite background(renderer, "data/sprite/background/plain.png", 1);
    std::vector<Option> options;
    pointToAlly = true;
    bool team1Alive = true;
    while (!quit)
    {
        if (timeLastFrame + 200 < SDL_GetTicks())
        {
            updateSprite(teamSpritesIcon1);
            timeLastFrame = SDL_GetTicks();
        }
        if (fight.isOver(team1Alive) && spellImpacts.size() == 0)
        {
            quit=true;
        }
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                return false;
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    returnToPreviousMenu();
                if (event.key.keysym.sym == SDLK_q)
                    quit = true;
                if (event.key.keysym.sym == SDLK_RETURN)
                {
                    getPlayerFightChoice();
                    checkChoiceSet();
                }
                if (event.key.keysym.sym == SDLK_UP)
                    menus[currentMenu].changeChoiceUp();
                if (event.key.keysym.sym == SDLK_DOWN)
                    menus[currentMenu].changeChoiceDown();
                if (event.key.keysym.sym == SDLK_LEFT)
                    menus[currentMenu].changeChoiceLeft();
                if (event.key.keysym.sym == SDLK_RIGHT)
                    menus[currentMenu].changeChoiceRight();
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        if (status != simulateTurn)
            pointToAlly = false;
        background.draw(0, 0, camera, false, 1, false, !pointToAlly);
        if (status == simulateTurn)
            simulateActions();
        drawTeam(pointToAlly);
        if (status != simulateTurn)
            menus[currentMenu].drawOptions(renderer, camera);
        drawOrder();
        drawTeamInfo(fight.team1, teamSpritesIcon1);
        SDL_RenderPresent(renderer);
    }
    return true;
}