#include "Game.h"

Game::Game()
{
    screenWidth = 800;
    screenHeight = 700;
    camera.x = 0;
    camera.y = 0;
    camera.w = screenWidth;
    camera.h = screenHeight;
    camera.zoom = 2;

    // Initialisation de la SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // Création de la fenêtre
    window = SDL_CreateWindow("Dragon quest monster SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }

    // Création du renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        SDL_DestroyWindow(window);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }

    // Initialisation de SDL_image
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        exit(1);
    }

    // Initialisation de SDL_ttf
    if (TTF_Init() == -1)
    {
        std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        exit(1);
    }

    // Initialisation de SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        exit(1);
    }

    musicVolume = 28;
    Mix_VolumeMusic(musicVolume);

    cursor = Sprite(renderer, "data/sprite/cursor/cursor_", 4);

    timeLastFrame = SDL_GetTicks();

    // Chargement de la base de données
    database.loadDatabase();

    loadGame();
    loadSprites();
    loadSounds();
    createMenus();
    std::vector<std::string> sprites;
    sprites.push_back("player_front");
    sprites.push_back("player_back");
    sprites.push_back("player_left");
    sprites.push_back("player_right");

    int x = save["mapInfo"]["x"].GetInt();
    int y = save["mapInfo"]["y"].GetInt();
    player.respawnMap = save["mapInfo"]["respawn"].GetString();

    playerEntity = Entity(x, y, 2, sprites, 16, 24, 8, 4);

    currentMap.loadMap("data/map/map.json", database);

    hitboxes = currentMap.getHitboxes();
    entities = currentMap.getEntities();
}

void Game::loadSounds()
{
    musics["overworld"] = Mix_LoadMUS("data/audio/music/overworld.mp3");
    musics["battle"] = Mix_LoadMUS("data/audio/music/monster_battle.mp3");
    sounds["victory"] = Mix_LoadWAV("data/audio/sounds/victory.wav");
    sounds["levelUp"] = Mix_LoadWAV("data/audio/sounds/level_up.wav");
    sounds["cast"] = Mix_LoadWAV("data/audio/sounds/cast.wav");
    sounds["inn"] = Mix_LoadWAV("data/audio/sounds/inn.wav");
}

void Game::loadSprites()
{
    sprites["player_front"] = Sprite(renderer, "data/sprite/player/player_front_", 3);
    sprites["player_back"] = Sprite(renderer, "data/sprite/player/player_back_", 3);
    sprites["player_left"] = Sprite(renderer, "data/sprite/player/player_left_", 3);
    sprites["player_right"] = Sprite(renderer, "data/sprite/player/player_right_", 3);

    for (size_t i = 0; i < player.mainTeam.size(); i++)
    {
        loadOverworldSprite(renderer, player.mainTeam[i].getInfos("type"), sprites);
        sprites[player.mainTeam[i].getInfos("type")] = Sprite(renderer, "data/sprite/fighting_sprite/" + player.mainTeam[i].getInfos("type") + ".png", 1);
    }
    for (size_t i = 0; i < player.benchTeam.size(); i++)
    {
        loadOverworldSprite(renderer, player.benchTeam[i].getInfos("type"), sprites);
        sprites[player.benchTeam[i].getInfos("type")] = Sprite(renderer, "data/sprite/fighting_sprite/" + player.benchTeam[i].getInfos("type") + ".png", 1);
    }
    for (size_t i = 0; i < player.storageMonsters.size(); i++)
    {
        loadOverworldSprite(renderer, player.storageMonsters[i].getInfos("type"), sprites);
        sprites[player.storageMonsters[i].getInfos("type")] = Sprite(renderer, "data/sprite/fighting_sprite/" + player.storageMonsters[i].getInfos("type") + ".png", 1);
    }

    sprites["../source/1.png"] = Sprite(renderer, "data/tileset/source/1.png", 1);
    sprites["../source/2.png"] = Sprite(renderer, "data/tileset/source/2.png", 1);
    sprites["../source/3.png"] = Sprite(renderer, "data/tileset/source/3.png", 1);
    sprites["../source/4.png"] = Sprite(renderer, "data/tileset/source/4.png", 1);
    sprites["../source/5.png"] = Sprite(renderer, "data/tileset/source/5.png", 1);
    sprites["../source/6.png"] = Sprite(renderer, "data/tileset/source/6.png", 1);
    sprites["../source/7.png"] = Sprite(renderer, "data/tileset/source/7.png", 1);
    sprites["../source/8.png"] = Sprite(renderer, "data/tileset/source/8.png", 1);
    sprites["../source/9.png"] = Sprite(renderer, "data/tileset/source/9.png", 1);
    sprites["../source/10.png"] = Sprite(renderer, "data/tileset/source/10.png", 1);

    sprites["pausescreen"] = Sprite(renderer, "data/sprite/filter/pause.png", 1);
    sprites["night"] = Sprite(renderer, "data/sprite/filter/night.png", 1);

    sprites["neutre"] = Sprite(renderer, "data/sprite/icon/neutre.png", 1);
    sprites["plus"] = Sprite(renderer, "data/sprite/icon/plus.png", 1);
    sprites["moins"] = Sprite(renderer, "data/sprite/icon/moins.png", 1);
}

Game::~Game()
{
    for (auto &music : musics)
    {
        Mix_FreeMusic(music.second);
    }
    for (auto &sound : sounds)
    {
        Mix_FreeChunk(sound.second);
    }
    Mix_CloseAudio();
    Mix_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::updateStorageMonsterMenu()
{
    menuManager.menus["monsterStorage"] = Menu(&cursor);
    int page = 0;
    size_t i = 0;
    std::vector<Option> options;
    while (i < player.storageMonsters.size())
    {
        options.clear();
        for (size_t j = i; j < std::min(i + 3, player.storageMonsters.size()); j++)
        {
            options.push_back(createOption("", 10 + (j - i) * 250, 550, 250, 100, UP, true));
        }
        menuManager.menus["monsterStorage"].addRow(options, page);
        page++;
        i += std::min((size_t)3, player.storageMonsters.size() - i);
    }
}

void Game::getSkillPointsMenu(Monster monster)
{
    Menu menu(&cursor);
    std::vector<Option> options;
    options.push_back(createOption("valider", 250, 630, 150, 50, UP));
    menu.addRow(options);
    for (size_t i = 0; i < monster.getSkills().size(); i++)
    {
        options.clear();
        options.push_back(createOption("+", 350, 350 + i * 75, 45, 40, UP));
        options.push_back(createOption("-", 400, 350 + i * 75, 45, 40, UP));
        menu.addRow(options);
    }
    menuManager.menus["skillPoints"] = menu;
}

void Game::createMenus()
{
    std::vector<Option> options, options2;
    menuManager.menus["pause"] = Menu(&cursor);
    options.push_back(createOption("Sauvegarder", 10, 50, 450, 50, RIGHT));
    menuManager.menus["pause"].addRow(options);
    options.clear();
    options.push_back(createOption("Changer de monstre", 10, 110, 450, 50, RIGHT));
    menuManager.menus["pause"].addRow(options);
    options.clear();
    options.push_back(createOption("Attribuer compétence", 10, 170, 450, 50, RIGHT));
    menuManager.menus["pause"].addRow(options);
    options.clear();
    options.push_back(createOption("Quitter", 10, 230, 450, 50, RIGHT));
    menuManager.menus["pause"].addRow(options);
    options.clear();

    menuManager.menus["yesNo"] = Menu(&cursor);
    options.push_back(createOption("Oui", 650, 490, 75, 35, LEFT, false, 12));
    menuManager.menus["yesNo"].addRow(options);
    options.clear();
    options.push_back(createOption("Non", 650, 520, 75, 35, LEFT, false, 12));
    menuManager.menus["yesNo"].addRow(options);

    menuManager.menus["swapTeam"] = Menu(&cursor);
    options.clear();
    for (size_t i = 0; i < 3; i++)
    {
        options.push_back(createOption("", 10 + i * 260, 440, 250, 100, UP, true));
        options2.push_back(createOption("", 10 + i * 260, 550, 250, 100, DOWN, true));
    }
    menuManager.menus["swapTeam"].addRow(options);
    menuManager.menus["swapTeam"].addRow(options2);
    options.clear();
    options2.clear();

    updateStorageMonsterMenu();

    options.clear();
    menuManager.menus["monsterCenter"] = Menu(&cursor);
    options.push_back(createOption("Synthétiser", 50, 50, 400, 50, RIGHT));
    menuManager.menus["monsterCenter"].addRow(options);
    options.clear();
    options.push_back(createOption("Envoyer en réserve", 50, 110, 400, 50, RIGHT));
    menuManager.menus["monsterCenter"].addRow(options);
    options.clear();
    options.push_back(createOption("Ajouter à l'équipe", 50, 170, 400, 50, RIGHT));
    menuManager.menus["monsterCenter"].addRow(options);
    options.clear();
    options.push_back(createOption("Se reposer", 50, 230, 400, 50, RIGHT));
    menuManager.menus["monsterCenter"].addRow(options);
    options.clear();

    menuManager.menus["scoutMonster"] = Menu(&cursor);
    options.push_back(createOption("Envoyer en réserve", 50, 510, 400, 50, RIGHT));
    menuManager.menus["scoutMonster"].addRow(options);
    options.clear();
    options.push_back(createOption("Ajouter à l'équipe", 50, 570, 400, 50, RIGHT));
    menuManager.menus["scoutMonster"].addRow(options);
    options.clear();
    options.push_back(createOption("Relâcher le monstre", 50, 630, 400, 50, RIGHT));
    menuManager.menus["scoutMonster"].addRow(options);
    options.clear();

    menuManager.menus["synthChoice"] = Menu(&cursor);
    options.push_back(createOption("Monstre +", 50, 510, 400, 50, RIGHT));
    menuManager.menus["synthChoice"].addRow(options);
    options.clear();
    options.push_back(createOption("Monstre -", 50, 570, 400, 50, RIGHT));
    menuManager.menus["synthChoice"].addRow(options);
    options.clear();
    options.push_back(createOption("Valider", 50, 630, 400, 50, RIGHT));
    menuManager.menus["synthChoice"].addRow(options);
    options.clear();
}

void Game::loadGame()
{
    loadJson("data/save/save.json", save);
    database.loadDatabase();
    for (size_t i = 0; i < save["mainTeam"].Size(); i++)
    {
        rapidjson::Value &monsterData = save["monsters"][save["mainTeam"][i].GetString()];
        Monster m = Monster(monsterData, save["mainTeam"][i].GetString(), database);
        player.mainTeam.push_back(m);
    }
    for (size_t i = 0; i < save["benchTeam"].Size(); i++)
    {
        rapidjson::Value &monsterData = save["monsters"][save["benchTeam"][i].GetString()];
        Monster m = Monster(monsterData, save["benchTeam"][i].GetString(), database);
        player.benchTeam.push_back(m);
    }
    for (size_t i = 0; i < save["storageMonsters"].Size(); i++)
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
    for (size_t i = 0; i < player.mainTeam.size(); i++)
    {
        player.mainTeam[i].updateSaveMonster(save);
        rapidjson::Value v(player.mainTeam[i].getInfos("id").c_str(), save.GetAllocator());
        idMain.PushBack(v, save.GetAllocator());
    }
    for (size_t i = 0; i < player.benchTeam.size(); i++)
    {
        player.benchTeam[i].updateSaveMonster(save);
        rapidjson::Value v(player.benchTeam[i].getInfos("id").c_str(), save.GetAllocator());
        idBench.PushBack(v, save.GetAllocator());
    }
    for (size_t i = 0; i < player.storageMonsters.size(); i++)
    {
        player.storageMonsters[i].updateSaveMonster(save);
        rapidjson::Value v(player.storageMonsters[i].getInfos("id").c_str(), save.GetAllocator());
        idStorage.PushBack(v, save.GetAllocator());
    }
    save["mainTeam"] = idMain;
    save["benchTeam"] = idBench;
    save["storageMonsters"] = idStorage;
    save["mapInfo"]["x"].SetInt(playerEntity.getX());
    save["mapInfo"]["y"].SetInt(playerEntity.getY());
    saveJson("data/save/save.json", save);
}

void Game::deleteFromSave(std::string id)
{
    for (size_t i = 0; i < save["mainTeam"].Size(); i++)
    {
        if (save["mainTeam"][i].GetString() == id)
        {
            save["mainTeam"].Erase(save["mainTeam"].Begin() + i);
            break;
        }
    }
    for (size_t i = 0; i < save["benchTeam"].Size(); i++)
    {
        if (save["benchTeam"][i].GetString() == id)
        {
            save["benchTeam"].Erase(save["benchTeam"].Begin() + i);
            break;
        }
    }
    for (size_t i = 0; i < save["storageMonsters"].Size(); i++)
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

void Game::drawHitbox(Hitbox h)
{
    SDL_Rect rect;
    rect.x = (h.getRealX() * camera.zoom) - camera.x;
    rect.y = (h.getRealY() * camera.zoom) - camera.y;
    rect.w = h.getWidth() * camera.zoom;
    rect.h = h.getHeight() * camera.zoom;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
}

void Game::drawMap()
{
    for (size_t i = 0; i < currentMap.getLayerCount() - 1; i++)
    {
        currentMap.drawLayer(i, renderer, camera, sprites, database);
    }
}

void Game::drawEntity(Entity e)
{
    if (e.getName() == "")
        return;
    if (sprites.find(e.getSpriteID()) == sprites.end())
    {
        loadOverworldSprite(renderer, e.getName(), sprites);
    }
    sprites[e.getSpriteID()].draw(e.getX(), e.getY(), camera, 1, e.getFrame(), false);
}

void Game::runScript(std::string scriptName)
{
    bool end = false;
    rapidjson::Value &script = database.getScript(scriptName);
    rapidjson::Value scriptCopy(script, save.GetAllocator());
    while (!end && running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = true;
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_RETURN)
                {
                    if (!scriptCopy.HasMember("next"))
                        end = true;
                    else
                        scriptCopy = scriptCopy["next"];
                }
            }
        }
        drawGame();
        updateCameraPosition();
        if (scriptCopy.HasMember("text"))
        {
            std::string text = scriptCopy["text"].GetString();
            std::string name = scriptCopy["name"].GetString();
            drawDialogBox(renderer, text, name);
        }
        if (scriptCopy.HasMember("menu"))
        {
            menuManager.menuStatus = scriptCopy["menuStatus"].GetString();
            openMenu(scriptCopy["menu"].GetString());
            if (!scriptCopy.HasMember("next"))
                end = true;
            else
                scriptCopy = scriptCopy["next"];
        }
        SDL_RenderPresent(renderer);
    }
}

void Game::getAction()
{
    Hitbox h;
    switch (playerEntity.getDirection())
    {
    case MOVE_UP:
        h = Hitbox(playerEntity.getX(), playerEntity.getY() - 16, 16, 24);
        break;
    case MOVE_DOWN:
        h = Hitbox(playerEntity.getX(), playerEntity.getY() + 16, 16, 24);
        break;
    case MOVE_LEFT:
        h = Hitbox(playerEntity.getX() - 16, playerEntity.getY(), 16, 24);
        break;
    case MOVE_RIGHT:
        h = Hitbox(playerEntity.getX() + 16, playerEntity.getY(), 16, 24);
        break;
    default:
        break;
    }
    for (size_t i = 0; i < entities.size(); i++)
    {
        if (entities[i].getType() == NPC)
        {
            if (h.isColliding(entities[i].getHitbox()))
            {
                if (entities[i].getScript() != "" && playerEntity.inFrontOf(entities[i]))
                    runScript(entities[i].getScript());
            }
        }
    }
}

void Game::getEvents()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            running = false;
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                menuManager.menuStatus = "pause";
                openMenu("pause");
            }
            if (event.key.keysym.sym == SDLK_UP)
                playerEntity.moves[MOVE_UP] = true;
            if (event.key.keysym.sym == SDLK_DOWN)
                playerEntity.moves[MOVE_DOWN] = true;
            if (event.key.keysym.sym == SDLK_LEFT)
                playerEntity.moves[MOVE_LEFT] = true;
            if (event.key.keysym.sym == SDLK_RIGHT)
                playerEntity.moves[MOVE_RIGHT] = true;
            if (event.key.keysym.sym == SDLK_t)
                camera.zoom++;
            if (event.key.keysym.sym == SDLK_g)
                camera.zoom--;
            if (event.key.keysym.sym == SDLK_m)
                adminView = !adminView;
            if (event.key.keysym.sym == SDLK_RETURN)
                getAction();
        }
        if (event.type == SDL_KEYUP)
        {
            if (event.key.keysym.sym == SDLK_UP)
                playerEntity.moves[MOVE_UP] = false;
            if (event.key.keysym.sym == SDLK_DOWN)
                playerEntity.moves[MOVE_DOWN] = false;
            if (event.key.keysym.sym == SDLK_LEFT)
                playerEntity.moves[MOVE_LEFT] = false;
            if (event.key.keysym.sym == SDLK_RIGHT)
                playerEntity.moves[MOVE_RIGHT] = false;
        }
    }
}

void Game::updateCameraPosition()
{
    camera.x = std::min(std::max(playerEntity.getX() * camera.zoom - (screenWidth / 2), 0), ((currentMap.getWidth()) * 32) * camera.zoom - screenWidth);
    camera.y = std::min(std::max(playerEntity.getY() * camera.zoom - (screenHeight / 2), 0), ((currentMap.getHeight()) * 32) * camera.zoom - screenHeight);
}

void Game::drawSkillSetInfo(std::string skillSet)
{
    rapidjson::Value &skillBase = database.getSkillBase()[skillSet.c_str()];
    int line = 0, column = 0;
    for (rapidjson::Value::ConstMemberIterator itr = skillBase["stepUpgrade"].MemberBegin(); itr != skillBase["stepUpgrade"].MemberEnd(); itr++)
    {
        std::string text = itr->name.GetString() + (std::string) ": " + std::to_string(itr->value["point"].GetInt());
        drawText(renderer, text, 30 + column * 300, 70 + line * 50, 16, {255, 255, 255});
        line++;
        if (line == 5)
        {
            line = 0;
            column++;
        }
    }
}

void Game::drawMonsterInfo(int x, int y, Monster m)
{
    int width = 780;
    int height = 350;
    SDL_Color white = {255, 255, 255};
    drawBox(renderer, x, y, width, height);
    loadMonsterSprite(renderer, m.getInfos("type"), sprites);
    sprites[m.getInfos("type")].draw(x + 390, y + 200, 2, -1, true);
    drawText(renderer, "Nom: " + m.getInfos("name"), x + 20, y + 20, 16, white);
    drawText(renderer, "Niv: " + std::to_string(m.getLevel()), x + 320, y + 20, 16, white);
    sprites[m.getInfos("charge")].draw(x + 500, y + 15);
    std::string rang = "Rang: " + m.getInfos("rank");
    if (m.getSynthLevel() > 0)
        rang += "+" + std::to_string(m.getSynthLevel());
    drawText(renderer, rang, x + 600, y + 20, 16, white);
    drawText(renderer, m.getInfos("type"), x + 20, y + 40, 16, white);
    drawText(renderer, "PV: " + std::to_string(m.getHp()) + "/" + std::to_string((int)m.getStat("hp")), x + 320, y + 60, 16, white);
    drawText(renderer, "PM: " + std::to_string(m.getMp()) + "/" + std::to_string((int)m.getStat("mp")), x + 600, y + 60, 16, white);
    drawText(renderer, "Attaque: " + createSpaceString(5) + std::to_string((int)m.getStat("atk")), x + 20, y + 100, 16, white);
    drawText(renderer, "Défense: " + createSpaceString(5) + std::to_string((int)m.getStat("def")), x + 20, y + 120, 16, white);
    drawText(renderer, "Agilité: " + createSpaceString(9) + std::to_string((int)m.getStat("agi")), x + 20, y + 140, 16, white);
    drawText(renderer, "Sagesse: " + createSpaceString(5) + std::to_string((int)m.getStat("wis")), x + 20, y + 160, 16, white);
    drawText(renderer, "Exp: " + std::to_string(m.getExp()), x + 20, y + 300, 16, white);
    drawText(renderer, "Exp niveau suiv.: " + std::to_string(m.getXpLvl(m.getLevel() + 1) - m.getExp()), x + 20, y + 320, 16, white);
    drawText(renderer, "Compétences:", x + 490, y + 100, 16, white);
    size_t i = 0;
    for (auto skill : m.getSkills())
    {
        drawText(renderer, skill.first, x + 490, y + 120 + i * 20, 16, white);
        i++;
    }
}

void Game::drawTeamInfo(int x, int y, std::vector<Monster> team, int shiftX, int shiftY)
{
    int width = 250;
    int height = 100;
    SDL_Color white = {255, 255, 255};
    for (size_t i = 0; i < team.size(); i++)
    {
        loadOverworldSprite(renderer, team[i].getInfos("type"), sprites);
        drawBox(renderer, x + i * shiftX, y + i * shiftY, width, height);
        drawText(renderer, "Nom: " + team[i].getInfos("name"), x + i * shiftX + 10, y + i * shiftY + 10, 12, white);
        sprites[team[i].getInfos("type") + "_front"].draw(x + i * shiftX + 10, y + i * shiftY + 25, 2);
        std::string hp = "PV: " + std::to_string(team[i].getHp()) + "/" + std::to_string((int)team[i].getStat("hp"));
        drawText(renderer, hp, x + i * shiftX + 80, y + i * shiftY + 35, 12, white);
        std::string mp = "PM: " + std::to_string(team[i].getMp()) + "/" + std::to_string((int)team[i].getStat("mp"));
        drawText(renderer, mp, x + i * shiftX + 80, y + i * shiftY + 55, 12, white);
    }
    for (size_t i = team.size(); i < 3; i++)
    {
        drawBox(renderer, x + i * shiftX, y + i * shiftY, width, height);
        drawText(renderer, "Vide", x + i * shiftX + 90, y + i * shiftY + 40, 12, white);
    }
}

void Game::drawPartTeamInfo(int x, int y, std::vector<Monster> team, int shiftX, int shiftY, int start, int end)
{
    for (size_t i = start; i < end; i++)
    {
        drawPartMonsterInfo(x + (i - start) * shiftX, y + (i - start) * shiftY, team[i]);
    }
}

void Game::drawPartTeamInfo(int x, int y, std::vector<Monster *> team, int shiftX, int shiftY, int start, int end)
{
    for (size_t i = start; i < end; i++)
    {
        drawPartMonsterInfo(x + (i - start) * shiftX, y + (i - start) * shiftY, *team[i]);
    }
}

void Game::drawPartMonsterInfo(int x, int y, Monster monster)
{
    int width = 250;
    int height = 100;
    SDL_Color white = {255, 255, 255};
    drawBox(renderer, x, y, width, height);
    drawText(renderer, "Nom: " + monster.getInfos("name"), x + 10, y + 10, 12, white);
    sprites[monster.getInfos("type") + "_front"].draw(x + 10, y + 25, 2);
    std::string hp = "PV: " + std::to_string(monster.getHp()) + "/" + std::to_string((int)monster.getStat("hp"));
    drawText(renderer, hp, x + 80, y + 35, 12, white);
    std::string mp = "PM: " + std::to_string(monster.getMp()) + "/" + std::to_string((int)monster.getStat("mp"));
    drawText(renderer, mp, x + 80, y + 55, 12, white);
}

void Game::drawGame()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    drawMap();
    for (size_t i = 0; i < entities.size(); i++)
    {
        drawEntity(entities[i]);
        if (adminView)
            drawHitbox(entities[i].getHitbox());
    }
    sprites[playerEntity.getSpriteID()].draw(playerEntity.getX(), playerEntity.getY(), camera, 1, playerEntity.getFrame(), false);
    currentMap.drawLayer(currentMap.getLayerCount() - 1, renderer, camera, sprites, database);
    if (adminView)
    {
        for (size_t i = 0; i < hitboxes.size(); i++)
        {
            drawHitbox(hitboxes[i]);
        }
        printInfos();
        drawHitbox(playerEntity.getHitbox());
    }
}

void Game::printInfos()
{
    SDL_Color color = {255, 255, 255};
    drawText(renderer, "X : " + std::to_string(playerEntity.getX()), 0, 0, 15, color);
    drawText(renderer, "Y : " + std::to_string(playerEntity.getY()), 0, 15, 15, color);
    drawText(renderer, "Camera X : " + std::to_string(camera.x), 0, 30, 15, color);
    drawText(renderer, "Camera Y : " + std::to_string(camera.y), 0, 45, 15, color);
    drawText(renderer, "Zoom : " + std::to_string(camera.zoom), 0, 60, 15, color);
    drawText(renderer, "screenWidth : " + std::to_string(screenWidth), 0, 75, 15, color);
    drawText(renderer, "screenHeight : " + std::to_string(screenHeight), 0, 90, 15, color);
    drawText(renderer, "Elapsed time : " + std::to_string(SDL_GetTicks() / 1000), 0, 105, 15, color);
}

void Game::startFight(std::string monster)
{
    playerEntity.clearMoves();
    unsigned int levelMin = 1;
    unsigned int levelMax = 10;
    std::vector<std::string> monsters = currentMap.getMonsters(levelMin, levelMax);
    Fight f(player.mainTeam, createWildMonsterTeam(monsters, levelMin, levelMax, monster));
    playMusic(musics["battle"]);
    FightSDL fight(f, renderer, &cursor, &database, &sprites, &sounds);
    bool alive = fight.runFight();
    if (alive)
    {
        player.mainTeam = fight.getTeam(1);
        if (fight.getScouted())
        {
            Monster monster = *fight.getMonsterScouted();
            monster.createSaveMonster(save);
            player.storageMonsters.push_back(monster);
            updateStorageMonsterMenu();
            menuManager.menuStatus = "scoutMonster";
            openMenu("scoutMonster", false);
        }
        else if (!fight.getFleed())
        {
            player.addGold(fight.getGoldReward());
            for (size_t i = 0; i < player.benchTeam.size(); i++)
            {
                if (player.benchTeam[i].getHp() > 0)
                    player.benchTeam[i].addXp(fight.getXpReward() / 2);
            }
        }
    }
    else
    {
        player.removeGold(player.getGold() / 2);
        player.healAllMonsters();
        currentMap.loadMap("data/map/" + player.respawnMap, database);
        unsigned int x = 0;
        unsigned int y = 0;
        currentMap.getRespawnCoord(x, y);
        playerEntity.move(x, y);
    }
    playMusic(musics["overworld"]);
}

void Game::updateEntities()
{
    playerEntity.update();
    playerEntity.clearCollision();
    for (size_t i = 0; i < hitboxes.size(); i++)
    {
        playerEntity.collision(hitboxes[i]);
    }
    size_t indexToErase = -1;
    for (size_t i = 0; i < entities.size(); i++)
    {
        if (tileInCamera(camera, entities[i].getX(), entities[i].getY()))
        {
            entities[i].updateAi();
            entities[i].clearCollision();
            for (size_t j = 0; j < hitboxes.size(); j++)
            {
                entities[i].collision(hitboxes[j]);
            }
            if (entities[i].getType() == NPC)
            {
                playerEntity.collision(entities[i].getHitbox());
            }
            if (entities[i].getType() == MONSTER && entities[i].collided(playerEntity))
            {
                startFight(entities[i].getName());
                indexToErase = i;
            }
        }
    }
    if (indexToErase != -1)
    {
        entities.erase(entities.begin() + indexToErase);
    }
}

void Game::drawMessage(std::string message)
{
    bool read = false;
    while (!read && running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
                read = true;
                inMenu = false;
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_ESCAPE)
                {
                    read = true;
                }
            }
        }
        drawGame();
        if (inMenu)
            sprites["pausescreen"].draw(0, 0);
        drawDialogBox(renderer, message, "");
        SDL_RenderPresent(renderer);
    }
}

bool Game::askForConfirmation(std::string message)
{
    bool read = false;
    bool confirmed = false;
    while (!read && running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
                read = true;
                inMenu = false;
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_UP)
                    menuManager.menus["yesNo"].changeChoiceUp();
                if (event.key.keysym.sym == SDLK_DOWN)
                    menuManager.menus["yesNo"].changeChoiceDown();
                if (event.key.keysym.sym == SDLK_RETURN)
                {
                    confirmed = menuManager.menus["yesNo"].getCurrentChoiceY() == 0;
                    read = true;
                }
            }
        }
        drawGame();
        if (inMenu)
            sprites["pausescreen"].draw(0, 0);
        drawDialogBox(renderer, message, "");
        menuManager.menus["yesNo"].drawOptions(renderer, camera);
        SDL_RenderPresent(renderer);
    }
    return confirmed;
}

std::vector<Monster> *Game::getTeamFromChoice(int choice)
{
    switch (choice)
    {
    case 0:
        return &player.mainTeam;
        break;
    case 1:
        return &player.benchTeam;
        break;
    default:
        return &player.mainTeam;
        break;
    }
}

void Game::clearSynthesis()
{
    synthetizer.mPlus = nullptr;
    synthetizer.mMinus = nullptr;
    synthetizer.children.clear();
    synthetizer.choiceMinus.clear();
    synthetizer.choicePlus.clear();
    synthetizer.skills.clear();
    synthetizer.skillsSelected.clear();
    synthetizer.child = "";

}

void Game::getSynthesisVector()
{
    synthetizer.choiceMinus.clear();
    synthetizer.choicePlus.clear();
    std::string idPlus = "";
    if (synthetizer.mPlus != nullptr)
    {
        idPlus = synthetizer.mPlus->getInfos("id");
    }
    std::string idMinus = "";
    if (synthetizer.mMinus != nullptr)
    {
        idMinus = synthetizer.mMinus->getInfos("id");
    }
    for (size_t i = 0; i < player.storageMonsters.size(); i++)
    {
        if (player.storageMonsters[i].getInfos("id") != idPlus && player.storageMonsters[i].getInfos("charge") != "plus")
        {
            if (player.storageMonsters[i].getLevel() >= 10)
                synthetizer.choiceMinus.push_back(&player.storageMonsters[i]);
        }
        if (player.storageMonsters[i].getInfos("id") != idMinus && player.storageMonsters[i].getInfos("charge") != "moins")
        {
            if (player.storageMonsters[i].getLevel() >= 10)
                synthetizer.choicePlus.push_back(&player.storageMonsters[i]);
        }
    }
}

void Game::getSynthesisMenu(std::string charge)
{
    menuManager.menus["synthetizer"] = Menu(&cursor);
    int page = 0;
    size_t i = 0;
    std::vector<Monster *> monsters;
    if (charge == "plus")
        monsters = synthetizer.choicePlus;
    else
        monsters = synthetizer.choiceMinus;
    std::vector<Option> options;
    while (i < monsters.size())
    {
        options.clear();
        for (size_t j = i; j < std::min(i + 3, monsters.size()); j++)
        {
            options.push_back(createOption("", 10 + (j - i) * 250, 550, 250, 100, UP, true));
        }
        menuManager.menus["synthetizer"].addRow(options, page);
        page++;
        i += std::min((size_t)3, monsters.size() - i);
    }
}

void Game::getSynthesisResultMenu()
{
    menuManager.menus["synthesisResult"] = Menu(&cursor);
    std::vector<Option> options;
    for (size_t i = 0; i < synthetizer.children.size(); i++)
    {
        options.clear();
        options.push_back(createOption(synthetizer.children[i], 10, 500 + i * 60, 500, 50, RIGHT));
        menuManager.menus["synthesisResult"].addRow(options);
    }
}

void Game::getSynthSkillMenu()
{
    menuManager.menus["synthSkill"] = Menu(&cursor);
    menuManager.menus["synthSkill"].addOption(createOption("Valider", 10, 610, 200, 50, RIGHT), 0);
    size_t index = 1;
    for (auto &skill : synthetizer.skills)
    {
        std::string text = skill.first + " : " + std::to_string(skill.second);
        menuManager.menus["synthSkill"].addOption(createOption(text, 10, 250 + index * 60, 450, 50, RIGHT), index);
        index++;
    }
}

void Game::getPlayerChoice()
{
    if (menuManager.menuStatus == "pause")
    {
        switch (menuManager.menus[menuManager.currentMenu].getCurrentChoiceY())
        {
        case 0:
            saveGame();
            drawMessage("Partie sauvegardée");
            break;
        case 1:
            if (player.mainTeam.size() == 1 && player.benchTeam.size() == 0)
            {
                drawMessage("Vous ne pouvez pas retirer votre dernier monstre de l'équipe|principale.");
            }
            else
            {
                changeMenu("swapTeam", "swapTeam");
                forbiddenTeamSwap();
            }
            break;
        case 2:
            changeMenu("swapTeam", "applySkillPoints");
            forbiddenTeamSwap();
            break;
        case 3:
            if (askForConfirmation("Voulez-vous vraiment quitter la partie ?|Toute progression non sauvegardée sera perdue."))
            {
                running = false;
                inMenu = false;
            }
            break;
        default:
            break;
        }
    }
    else if (menuManager.menuStatus == "swapTeam")
    {
        if (menuManager.stepMenu == 1)
        {
            std::vector<Monster> *team1 = getTeamFromChoice(menuManager.previousChoiceY.top());
            std::vector<Monster> *team2 = getTeamFromChoice(menuManager.menus[menuManager.currentMenu].getCurrentChoiceY());
            swapMonsters(menuManager.previousChoiceX.top(), menuManager.menus[menuManager.currentMenu].getCurrentChoiceX(), team1, team2);
            returnToPreviousMenu();
        }
        else
        {
            menuManager.stepMenu++;
            menuManager.previousChoiceX.push(menuManager.menus[menuManager.currentMenu].getCurrentChoiceX());
            menuManager.previousChoiceY.push(menuManager.menus[menuManager.currentMenu].getCurrentChoiceY());
            if (player.mainTeam.size() == 1 && getTeamFromChoice(menuManager.previousChoiceY.top()) == &player.mainTeam)
                forbiddenTeamSwap(true);
            else
                forbiddenTeamSwap(false);
        }
    }
    else if (menuManager.menuStatus == "monsterCenter")
    {
        switch (menuManager.menus[menuManager.currentMenu].getCurrentChoiceY())
        {
        case 0:
            if (player.storageMonsters.size() >= 2)
            {
                clearSynthesis();
                changeMenu("synthChoice", "synthetizer");
            }
            else
            {
                drawMessage("Vous n'avez pas assez de monstres en réserve|à synthétiser.");
            }
            break;
        case 1:
            if (player.mainTeam.size() == 1 && player.benchTeam.size() == 0)
            {
                drawMessage("Vous ne pouvez pas retirer votre dernier monstre de l'équipe|principale.");
                break;
            }
            changeMenu("swapTeam", "storeMonster");
            forbiddenTeamSwap();
            break;
        case 2:
            if (player.storageMonsters.size() == 0)
            {
                drawMessage("Vous n'avez aucun monstre en réserve.");
                break;
            }
            changeMenu("monsterStorage", "addMonster");
            menuManager.menus["swapTeam"].clearForbiddenChoices();
            break;
        case 3:
            player.healAllMonsters();
            playSound(sounds["inn"]);
            drawMessage("Après un repos bien mérité, toute votre|équipe est guérit.");
        default:
            break;
        }
    }
    else if (menuManager.menuStatus == "storeMonster")
    {
        int teamChoice = menuManager.menus[menuManager.currentMenu].getCurrentChoiceY();
        int monsterIndex = menuManager.menus[menuManager.currentMenu].getCurrentChoiceX();
        std::vector<Monster> *team = getTeamFromChoice(teamChoice);
        player.storageMonsters.push_back((*team)[monsterIndex]);
        drawMessage((*team)[monsterIndex].getInfos("name") + " a été envoyé en réserve.");
        team->erase(team->begin() + monsterIndex);
        updateStorageMonsterMenu();
        player.healStorageMonsters();
        returnToPreviousMenu();
    }
    else if (menuManager.menuStatus == "addMonster")
    {
        if (menuManager.stepMenu == 1)
        {
            std::vector<Monster> *team1 = &player.storageMonsters;
            std::vector<Monster> *team2 = getTeamFromChoice(menuManager.menus[menuManager.currentMenu].getCurrentChoiceY());
            swapMonsters(menuManager.previousChoiceX.top(), menuManager.menus[menuManager.currentMenu].getCurrentChoiceX(), team1, team2);
            updateStorageMonsterMenu();
            player.healStorageMonsters();
            returnToPreviousMenu();
        }
        else
        {
            menuManager.stepMenu++;
            menuManager.previousChoiceX.push(menuManager.menus[menuManager.currentMenu].getCurrentChoiceX() + menuManager.menus[menuManager.currentMenu].getCurrentPage() * 3);
            menuManager.previousChoiceY.push(menuManager.menus[menuManager.currentMenu].getCurrentChoiceY());
            menuManager.currentMenu = "swapTeam";
        }
    }
    else if (menuManager.menuStatus == "applySkillPoints")
    {
        if (menuManager.stepMenu == 1)
        {
            if (menuManager.menus[menuManager.currentMenu].getCurrentChoiceY() == 0)
            {
                int index = 1;
                for (auto &skillSets : skillUpdater.monster->getSkills())
                {
                    skillUpdater.monster->applySkillPoint(skillUpdater.pointToApply.at(index), skillSets.first, database);
                    index++;
                }
                returnToPreviousMenu();
            }
            else
            {
                if (menuManager.menus[menuManager.currentMenu].getCurrentChoiceX() == 0)
                {
                    if (skillUpdater.pointLeft > 0)
                    {
                        skillUpdater.pointToApply[menuManager.menus[menuManager.currentMenu].getCurrentChoiceY()]++;
                        skillUpdater.pointLeft--;
                    }
                }
                else
                {
                    if (skillUpdater.pointToApply[menuManager.menus[menuManager.currentMenu].getCurrentChoiceY()] > 0)
                    {
                        skillUpdater.pointToApply[menuManager.menus[menuManager.currentMenu].getCurrentChoiceY()]--;
                        skillUpdater.pointLeft++;
                    }
                }
            }
        }
        else
        {
            menuManager.stepMenu++;
            std::vector<Monster> *team2 = getTeamFromChoice(menuManager.menus[menuManager.currentMenu].getCurrentChoiceY());
            getSkillPointsMenu((*team2)[menuManager.menus[menuManager.currentMenu].getCurrentChoiceX()]);
            skillUpdater.monster = &(*team2)[menuManager.menus[menuManager.currentMenu].getCurrentChoiceX()];
            for (size_t i = 0; i < skillUpdater.monster->getSkills().size(); i++)
            {
                skillUpdater.pointToApply[i] = 0;
            }
            skillUpdater.pointLeft = skillUpdater.monster->getSkillToAttribute();
            menuManager.currentMenu = "skillPoints";
        }
    }
    else if (menuManager.menuStatus == "scoutMonster")
    {
        if (menuManager.stepMenu == 1)
        {
            std::vector<Monster> *team = getTeamFromChoice(menuManager.menus[menuManager.currentMenu].getCurrentChoiceY());
            swapMonsters(player.storageMonsters.size() - 1, menuManager.menus[menuManager.currentMenu].getCurrentChoiceX(), &player.storageMonsters, team);
            updateStorageMonsterMenu();
            player.healStorageMonsters();
            emptyMenu();
            inMenu = false;
        }
        else
        {
            if (menuManager.menus[menuManager.currentMenu].getCurrentChoiceY() == 2)
            {
                drawMessage("Vous relâchez le monstre.");
                player.storageMonsters.pop_back();
                updateStorageMonsterMenu();
                emptyMenu();
                inMenu = false;
            }
            else if (menuManager.menus[menuManager.currentMenu].getCurrentChoiceY() == 1)
            {
                menuManager.stepMenu++;
                menuManager.previousChoiceX.push(menuManager.menus[menuManager.currentMenu].getCurrentChoiceX());
                menuManager.previousChoiceY.push(menuManager.menus[menuManager.currentMenu].getCurrentChoiceY());
                menuManager.currentMenu = "swapTeam";
            }
            else
            {
                drawMessage("Monstre envoyé en réserve");
                emptyMenu();
                inMenu = false;
            }
        }
    }
    else if (menuManager.menuStatus == "synthetizer")
    {
        if (menuManager.currentMenu == "synthChoice")
        {
            if (menuManager.menus[menuManager.currentMenu].getCurrentChoiceY() == 0)
            {
                getSynthesisVector();
                synthetizer.mPlus = nullptr;
                if (synthetizer.choicePlus.size() == 0)
                {
                    drawMessage("Vous n'avez pas de monstre de signe + synthétisable.");
                    returnToPreviousMenu();
                }
                else
                {
                    synthetizer.choosingPlus = true;
                    getSynthesisMenu("plus");
                    menuManager.currentMenu = "synthetizer";
                }
            }
            else if (menuManager.menus[menuManager.currentMenu].getCurrentChoiceY() == 1)
            {
                getSynthesisVector();
                synthetizer.mMinus = nullptr;
                if (synthetizer.choiceMinus.size() == 0)
                {
                    drawMessage("Vous n'avez pas de monstre de signe - synthétisable.");
                    returnToPreviousMenu();
                }
                else
                {
                    synthetizer.choosingPlus = false;
                    getSynthesisMenu("moins");
                    menuManager.currentMenu = "synthetizer";
                }
            }
            else if (menuManager.menus[menuManager.currentMenu].getCurrentChoiceY() == 2)
            {
                if (synthetizer.mPlus == nullptr)
                    drawMessage("Veuillez selectionner un monstre de signe plus");
                else if (synthetizer.mMinus == nullptr)
                    drawMessage("Veuillez selectionner un monstre  de signe moins");
                else
                {
                    synthetizer.children.clear();
                    synthetizer.children = makeSynthesisPreview(*synthetizer.mPlus, *synthetizer.mMinus, database);
                    getSynthesisResultMenu();
                    menuManager.currentMenu = "synthesisResult";
                }
            }
        }
        else if (menuManager.currentMenu == "synthetizer")
        {
            if (synthetizer.choosingPlus)
            {
                size_t index = menuManager.menus[menuManager.currentMenu].getCurrentChoiceX() + menuManager.menus[menuManager.currentMenu].getCurrentPage() * 3;
                synthetizer.mPlus = synthetizer.choicePlus[index];
            }
            else
            {
                size_t index = menuManager.menus[menuManager.currentMenu].getCurrentChoiceX() + menuManager.menus[menuManager.currentMenu].getCurrentPage() * 3;
                synthetizer.mMinus = synthetizer.choiceMinus[index];
            }
            menuManager.currentMenu = "synthChoice";
        }
        else if (menuManager.currentMenu == "synthesisResult")
        {
            synthetizer.child = synthetizer.children[menuManager.menus[menuManager.currentMenu].getCurrentChoiceY()];
            synthetizer.skills = getSynthesisSkills(*synthetizer.mPlus, *synthetizer.mMinus, synthetizer.child, database);
            getSynthSkillMenu();
            menuManager.currentMenu = "synthSkill";
        }
        else if (menuManager.currentMenu == "synthSkill")
        {
            if (menuManager.menus[menuManager.currentMenu].getCurrentChoiceY() == 0)
            {
                if (synthetizer.skillsSelected.size() == 0)
                {
                    drawMessage("Veuillez selectionner au moins un set de compétence");
                }
                else
                {
                    std::unordered_map<std::string, unsigned int> skillSets;
                    for (size_t i = 0; i < synthetizer.skillsSelected.size(); i++)
                    {
                        skillSets[synthetizer.skillsSelected[i]] = synthetizer.skills[synthetizer.skillsSelected[i]];
                    }
                    drawMessage("Synthétisation en cours...");
                    drawMessage("Synthétisation réussie !");
                    Monster newMonster = Monster(*synthetizer.mPlus, *synthetizer.mMinus, maxLenString(synthetizer.child,15), synthetizer.child, database, skillSets, save);
                    drawMessage("Vous obtenez un " + newMonster.getInfos("type") + " !");
                    size_t indexPlusToErase = 0;
                    size_t indexMinusToErase = 0;
                    for (size_t i = 0; i < player.storageMonsters.size(); i++)
                    {
                        if (player.storageMonsters[i] == *synthetizer.mPlus)
                        {
                            indexPlusToErase = i;
                        }
                        if (player.storageMonsters[i] == *synthetizer.mMinus)
                        {
                            indexMinusToErase = i;
                        }
                    }
                    player.storageMonsters.erase(player.storageMonsters.begin() + indexPlusToErase);
                    player.storageMonsters.erase(player.storageMonsters.begin() + indexMinusToErase);
                    player.storageMonsters.push_back(newMonster);
                    while (menuManager.previousMenu.size() > 0)
                    {
                        menuManager.previousMenu.pop();
                    }
                    menuManager.menuStatus = "scoutMonster";
                    openMenu("scoutMonster", false);
                }
            }
            else
            {
                if (synthetizer.skillsSelected.size() == 3)
                {
                    drawMessage("Vous ne pouvez pas selectionner plus de 3 sets de compétences");
                    return;
                }
                size_t index = menuManager.menus[menuManager.currentMenu].getCurrentChoiceY() - 1;
                size_t i = 0;
                for (auto &skill : synthetizer.skills)
                {
                    if (i == index)
                    {
                        synthetizer.skillsSelected.push_back(skill.first);
                        synthetizer.skills.erase(skill.first);
                        break;
                    }
                    i++;
                }
                getSynthSkillMenu();
            }
        }
    }
}

void Game::emptyMenu()
{
    while (menuManager.previousMenu.size() > 0)
    {
        returnToPreviousMenu();
    }
}

void Game::forbiddenTeamSwap(bool forbidden)
{
    menuManager.menus["swapTeam"].clearForbiddenChoices();
    if (forbidden)
    {
        for (size_t i = player.mainTeam.size(); i < 3; i++)
        {
            menuManager.menus["swapTeam"].setForbiddenChoice(i, 0, true);
        }
        for (size_t i = player.benchTeam.size(); i < 3; i++)
        {
            menuManager.menus["swapTeam"].setForbiddenChoice(i, 1, true);
        }
        if (player.mainTeam.size() == 1 && player.benchTeam.size() > 0) {
            menuManager.menus["swapTeam"].setForbiddenChoice(0,0,true);
        }
    }
    menuManager.menus["swapTeam"].setFirstChoice();
}

void Game::returnToPreviousMenu()
{
    for (auto &menu : menuManager.menus)
        menu.second.setFirstChoice();
    if (menuManager.previousMenu.empty())
        return;
    menuManager.currentMenu = menuManager.previousMenu.top();
    menuManager.previousMenu.pop();
    menuManager.menuStatus = menuManager.previousMenuStatus.top();
    menuManager.previousMenuStatus.pop();
    if (menuManager.previousChoiceX.empty() || menuManager.previousChoiceY.empty())
        return;
    menuManager.previousChoiceX.pop();
    menuManager.previousChoiceY.pop();
}

void Game::openMenu(std::string name, bool canEscape)
{
    inMenu = true;
    menuManager.currentMenu = name;
    menuManager.stepMenu = 0;
    playerEntity.clearMoves();
    while (inMenu && running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
                inMenu = false;
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_UP)
                    menuManager.menus[menuManager.currentMenu].changeChoiceUp();
                if (event.key.keysym.sym == SDLK_DOWN)
                    menuManager.menus[menuManager.currentMenu].changeChoiceDown();
                if (event.key.keysym.sym == SDLK_LEFT)
                    menuManager.menus[menuManager.currentMenu].changeChoiceLeft();
                if (event.key.keysym.sym == SDLK_RIGHT)
                    menuManager.menus[menuManager.currentMenu].changeChoiceRight();
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    if (menuManager.previousMenu.empty() && canEscape)
                        inMenu = false;
                    returnToPreviousMenu();
                }
                if (event.key.keysym.sym == SDLK_RETURN)
                {
                    getPlayerChoice();
                }
            }
        }
        drawGame();
        sprites["pausescreen"].draw(0, 0);
        if (menuManager.currentMenu == "pause")
        {
            drawTeamInfo(500, 50, player.mainTeam, 0, 110);
        }
        if (menuManager.currentMenu == "swapTeam")
        {
            std::vector<Monster> *team = getTeamFromChoice(menuManager.menus[menuManager.currentMenu].getCurrentChoiceY());
            int index = menuManager.menus[menuManager.currentMenu].getCurrentChoiceX();
            if (team->size() > index)
                drawMonsterInfo(10, 10, (*team)[index]);
            drawTeamInfo(10, 440, player.mainTeam, 260, 0);
            drawTeamInfo(10, 550, player.benchTeam, 260, 0);
        }
        if (menuManager.currentMenu == "monsterStorage")
        {
            int min = menuManager.menus["monsterStorage"].getCurrentPage() * 3;
            int max = min + std::min(3, (int)player.storageMonsters.size() - min);
            int monsterIndex = menuManager.menus["monsterStorage"].getCurrentChoiceX() + menuManager.menus["monsterStorage"].getCurrentPage() * 3;
            drawMonsterInfo(10, 10, player.storageMonsters[monsterIndex]);
            drawPartTeamInfo(10, 550, player.storageMonsters, 260, 0, min, max);
        }
        if (menuManager.currentMenu == "skillPoints")
        {
            SDL_Color color = {255, 255, 255};
            int i = 1;
            drawBox(renderer, 0, 0, 800, 700);
            drawText(renderer, "Points restants: " + std::to_string(skillUpdater.pointLeft), 30, 30, 20, color);
            for (auto &skill : skillUpdater.monster->getSkills())
            {
                std::string text = skill.first + ": " + std::to_string(skill.second) + " + " + std::to_string(skillUpdater.pointToApply[i]);
                drawText(renderer, text, 10, 350 + (i - 1) * 75, 15, color);
                i++;
                if (i == menuManager.menus["skillPoints"].getCurrentChoiceY() + 1)
                    drawSkillSetInfo(skill.first);
            }
        }
        if (menuManager.currentMenu == "scoutMonster")
        {
            drawMonsterInfo(10, 10, player.storageMonsters[player.storageMonsters.size() - 1]);
        }
        if (menuManager.currentMenu == "synthetizer")
        {
            std::vector<Monster *> monsters;
            if (synthetizer.choosingPlus)
                monsters = synthetizer.choicePlus;
            else
                monsters = synthetizer.choiceMinus;
            int min = menuManager.menus["synthetizer"].getCurrentPage() * 3;
            int max = min + std::min(3, (int)monsters.size() - min);
            int monsterIndex = menuManager.menus["synthetizer"].getCurrentChoiceX() + menuManager.menus["synthetizer"].getCurrentPage() * 3;
            drawMonsterInfo(10, 10, (*monsters[monsterIndex]));
            drawPartTeamInfo(10, 550, monsters, 260, 0, min, max);
        }
        if (menuManager.currentMenu == "synthChoice")
        {
            SDL_Color white = {255, 255, 255};
            drawBox(renderer, 0, 0, 800, 450);
            drawText(renderer, "Parent ", 70, 70, 20, white);
            drawText(renderer, "Parent ", 520, 70, 20, white);
            sprites["plus"].draw(210, 65);
            sprites["moins"].draw(660, 65);
            if (synthetizer.mPlus != nullptr)
            {
                loadMonsterSprite(renderer, synthetizer.mPlus->getInfos("type"), sprites);
                sprites[synthetizer.mPlus->getInfos("type")].draw(150, 200, 2, -1, true);
            }
            if (synthetizer.mMinus != nullptr)
            {
                loadMonsterSprite(renderer, synthetizer.mMinus->getInfos("type"), sprites);
                sprites[synthetizer.mMinus->getInfos("type")].draw(580, 200, 2, -1, true);
            }
        }
        if (menuManager.currentMenu == "synthesisResult")
        {
            drawBox(renderer, 0, 0, 800, 450);
            size_t index = menuManager.menus["synthesisResult"].getCurrentChoiceY();
            loadMonsterSprite(renderer, synthetizer.children[index], sprites);
            sprites[synthetizer.children[index]].draw(400, 200, 3, -1, true);
        }
        if (menuManager.currentMenu == "synthSkill")
        {
            drawBox(renderer, 0, 0, 800, 450);
            drawText(renderer, "Set de compétence sélectionnés:", 10, 30, 20, {255, 255, 255});
            for (size_t i = 0; i < synthetizer.skillsSelected.size(); i++)
            {
                drawText(renderer, "-" + synthetizer.skillsSelected[i], 100, 100 + i * 40, 20, {255, 255, 255});
            }
        }
        menuManager.menus[menuManager.currentMenu].drawOptions(renderer, camera);
        updateCameraPosition();
        SDL_RenderPresent(renderer);
    }
}

void Game::changeMenu(std::string name, std::string status)
{
    menuManager.previousMenu.push(menuManager.currentMenu);
    menuManager.previousMenuStatus.push(menuManager.menuStatus);
    menuManager.currentMenu = name;
    menuManager.menuStatus = status;
    menuManager.stepMenu = 0;
}

void Game::run()
{
    running = true;
    playMusic(musics["overworld"]);
    while (running)
    {
        updateEntities();
        // checkTextures();
        getEvents();
        drawGame();
        updateCameraPosition();
        SDL_RenderPresent(renderer);
    }
}

void Game::checkTextures()
{
    bool error = false;
    std::cout << "Checking textures..." << std::endl;
    for (auto &sprite : sprites)
    {
        std::vector<SDL_Texture *> textures = sprite.second.getTextures();
        for (size_t i = 0; i < textures.size(); i++)
        {
            if (SDL_QueryTexture(textures[i], NULL, NULL, NULL, NULL) != 0)
            {
                std::cout << sprite.second.getPath() << " " << SDL_GetError() << std::endl;
                error = true;
            }
        }
    }
    assert(!error);
    std::cout << "Textures checked" << std::endl;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    Game game;
    game.run();
    return 0;
}