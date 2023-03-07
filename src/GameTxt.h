#ifndef _GAME
#define _GAME

#include "Player.h"
#include "jsonFunction.h"
#include "Fight.h"

class Game
{
    public:
        Player player;
        rapidjson::Document monsterBase;
        rapidjson::Document save;
        rapidjson::Document skillBase;
        rapidjson::Document spellBase;
        
        /// @brief  Charge les données de save.json, skill.json, spell.json et monster.json
        void loadGame();

        /// @brief  Sauvegarde les données dans save.json, skill.json, spell.json et monster.json
        void saveGame();


        unsigned int askTarget(bool isOffensive,Fight& f);

        std::vector<Monster> getTargetPlayer(Monster caster, std::string spell,Fight& f);

        unsigned int orderMonster(Monster m);

        std::vector<Action> getPlayerChoice(Fight& f);

        void printOrder(std::vector<Action> order, Fight& f);

        void fight(Fight& f);


};

#endif