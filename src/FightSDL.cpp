
#include "FightSDL.h"

FightSDL::FightSDL(Fight &fight, SDL_Renderer *renderer, Sprite *cursor)
{
    this->fight = fight;
    this->renderer = renderer;
    this->cursor = cursor;
    this->teamSprites1 = createTeamSprite(fight.team1);
    this->teamSprites2 = createTeamSprite(fight.team2);
    this->teamSpritesIcon1 = createTeamSpriteIcon(fight.team1);
    this->timeLastFrame = SDL_GetTicks();
    this->camera.x = 0;
    this->camera.y = 0;
    this->caster = -1;
    this->tacticIcons = Sprite(renderer, "data/sprite/tactic_icon/tactic_", 4);
    createMenu();
    currentMenu = "main";
    tacticIndex = nullTactic;
    this->status = mainMenu;
}

FightSDL::~FightSDL()
{
}

void FightSDL::drawTeam(std::vector<Sprite> &team)
{
    float size = 3;
    switch (team.size())
    {
    case 1:
        team[0].draw(400, 350, camera, false, size, true);
        break;
    case 2:
        team[0].draw(300, 350, camera, false, size, true);
        team[1].draw(500, 350, camera, false, size, true);
        break;
    case 3:
        team[0].draw(200, 350, camera, false, size, true);
        team[1].draw(400, 350, camera, false, size, true);
        team[2].draw(600, 350, camera, false, size, true);
        break;
    default:
        break;
    }
}

std::vector<Sprite> FightSDL::createTeamSprite(std::vector<Monster> &team)
{
    std::vector<Sprite> teamSprite;
    for (long unsigned int i = 0; i < team.size(); i++)
    {
        Sprite s = Sprite(renderer, "data/sprite/fighting_sprite/" + team[i].getInfos("type") + ".png", 1);
        teamSprite.push_back(s);
    }
    return teamSprite;
}

std::vector<Sprite> FightSDL::createTeamSpriteIcon(std::vector<Monster> &team)
{
    std::vector<Sprite> teamSprite;
    for (long unsigned int i = 0; i < team.size(); i++)
    {
        Sprite s = Sprite(renderer, "data/sprite/overworld_monsters/" + team[i].getInfos("type") + "/" + team[i].getInfos("type") + "_front_", 3);
        teamSprite.push_back(s);
    }
    return teamSprite;
}

void FightSDL::drawTeamInfo(std::vector<Monster> &team, std::vector<Sprite> &teamIcons)
{
    int x = 10;
    int y = 10;
    int width = 250;
    int height = 100;
    SDL_Color white = {255, 255, 255};
    for (long unsigned int i = 0; i < team.size(); i++)
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

bool FightSDL::getPlayerFightChoice()
{
    switch (menus[currentMenu].getChoice())
    {
    case tacticMenu:
        changeCurrentMenu("team");
        caster = -1;
        status = tactic;
        return false;
    case casterIndex:
        caster = menus[currentMenu].getCurrentChoiceX();
        return false;
    case targetIndex:
        target.push_back(menus[currentMenu].getCurrentChoiceX());
        return false;
    case orderMenu:
        changeCurrentMenu("enemy");
        caster = -1;
        target.clear();
        status = order;
        return false;
    default:
        return false;
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
    menu.addRow(createSingleOptionRow(createOption("Combattre", 10, 450, 220, 45, RIGHT)));
    menu.addRow(createSingleOptionRow(createOption("Tactique", 10, 500, 220, 45, RIGHT, tacticMenu)));
    menu.addRow(createSingleOptionRow(createOption("Ordre", 10, 550, 220, 45, RIGHT, orderMenu)));
    menu.addRow(createSingleOptionRow(createOption("Dressage", 10, 600, 220, 45, RIGHT)));
    menu.addRow(createSingleOptionRow(createOption("Fuir", 10, 650, 220, 45, RIGHT)));
    menu.changePageX = false;
    menus["main"] = menu;
    menu.clear();
    for (long unsigned int i = 0; i < teamSprites1.size(); i++)
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
    for (long unsigned int i = 0; i < fight.team1.size(); i++)
    {
        menu.clear();
        int page = 0;
        long unsigned int spell = 0;
        std::vector<std::string> spells = fight.team1[i].getSpells();
        while (spell < spells.size())
        {
            for (int j = 0; j < std::min(spells.size(), 8 + spell); j++)
            {
                menu.addRow(createSingleOptionRow(createOption(spells[j], 10 + j * 260, 10 + page * 100, 250, 100, DOWN, spellIndex)), page);
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
        target.clear();
        this->status = mainMenu;
        return;
    }

    currentMenu = previousMenu.top();
    previousMenu.pop();
}

void FightSDL::checkChoiceSet()
{
    switch (status)
    {
    case tactic:
        if (caster != -1 && currentMenu == "tactic")
        {
            tacticIndex = (Tactics)menus[currentMenu].getCurrentChoiceY();
            fight.team1[caster].setTactic(tacticIndex);
            caster = -1;
            tacticIndex = nullTactic;
            status = tactic;
            returnToPreviousMenu();
        }
        else if (caster != -1)
        {
            //changeCurrentMenu("tactic");
            changeCurrentMenu("spell0");
        }
        break;
    default:
        break;
    }
}

bool FightSDL::runFight()
{
    bool quit = false;
    SDL_Event event;
    Sprite background(renderer, "data/sprite/background/plain.png", 1);
    std::vector<Option> options;
    bool pointToEnemy = true;
    while (!quit)
    {
        if (timeLastFrame + 200 < SDL_GetTicks())
        {
            updateSprite(teamSpritesIcon1);
            timeLastFrame = SDL_GetTicks();
        }
        while (SDL_PollEvent(&event))
        {
            pointToEnemy = true;
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
                    menus[currentMenu].changeCurrentChoice(menus[currentMenu].getCurrentChoiceX(), menus[currentMenu].getCurrentChoiceY() - 1);
                if (event.key.keysym.sym == SDLK_DOWN)
                    menus[currentMenu].changeCurrentChoice(menus[currentMenu].getCurrentChoiceX(), menus[currentMenu].getCurrentChoiceY() + 1);
                if (event.key.keysym.sym == SDLK_LEFT)
                    menus[currentMenu].changeCurrentChoice(menus[currentMenu].getCurrentChoiceX() - 1, menus[currentMenu].getCurrentChoiceY());
                if (event.key.keysym.sym == SDLK_RIGHT)
                    menus[currentMenu].changeCurrentChoice(menus[currentMenu].getCurrentChoiceX() + 1, menus[currentMenu].getCurrentChoiceY());
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        background.draw(0, 0, camera, false, 1, false, !pointToEnemy);
        if (pointToEnemy)
            drawTeam(teamSprites2);
        else
            drawTeam(teamSprites1);
        drawTeamInfo(fight.team1, teamSpritesIcon1);
        menus[currentMenu].drawOptions(renderer, camera);
        SDL_RenderPresent(renderer);
    }
    return true;
}