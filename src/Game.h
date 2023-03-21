#ifndef _GAME
#define _GAME

#include "Sprite.h"
#include "Player.h"
#include "Monster.h"
#include "Database.h"
#include "Synthesis.h"
#include "Fight.h"

class Game
{
    private:
    Player player;            // Le joueur
    rapidjson::Document save; // Les données de sauvegarde
    Database database;        // La base de données

    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_Event event;

    std::vector<Sprite*> sprites;

    public:
    Game();
    ~Game();


};


#endif