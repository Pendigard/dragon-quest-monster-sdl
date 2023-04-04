#include "Game.h"

Game::Game()
{
    screenWidth = 800;
    screenHeight = 700;
    camera.x = 0;
    camera.y = 0;
    camera.w = screenWidth;
    camera.h = screenHeight;

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

    cursor = Sprite(renderer, "data/sprite/cursor/cursor_", 4);

    timeLastFrame = SDL_GetTicks();

    // Chargement de la base de données
    database.loadDatabase();

    loadGame();
    loadSprites();
}

void Game::loadSprites()
{
    sprites["player_front"] = new Sprite(renderer, "data/sprite/player/player_front_", 3);  
    sprites["player_back"] = new Sprite(renderer, "data/sprite/player/player_back_", 3);
    sprites["player_left"] = new Sprite(renderer, "data/sprite/player/player_left_", 3);
    sprites["player_right"] = new Sprite(renderer, "data/sprite/player/player_right_", 3);
}

Game::~Game()
{
    for (auto &sprite : sprites)
    {
        delete sprite.second;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

#include "GameTxt.h"

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
    saveJson("data/save/saveTest.json", save);
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

void Game::run()
{
    SDL_Event event;
    running = true;
    player.mainTeam[0].applySkillPoint(100,"Estark",database);
    for (size_t i = 1; i < player.mainTeam.size(); i++)
    {
        player.mainTeam[i].autoAttributeSkill(database);
    }
    player.mainTeam[0].addXp(170000);
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                if (event.key.keysym.sym == SDLK_f)
                {
                    std::vector<std::string> monsters;
                    monsters.push_back("komodor");
                    monsters.push_back("gluant");
                    monsters.push_back("medhigluant");
                    monsters.push_back("jaunyve");
                    monsters.push_back("gluanbulle");
                    Fight f(player.mainTeam, createWildMonsterTeam(monsters, 47, 50));
                    FightSDL fight(f, renderer, &cursor, &database);
                    fight.runFight();
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    Game game;
    game.run();
    return 0;
}