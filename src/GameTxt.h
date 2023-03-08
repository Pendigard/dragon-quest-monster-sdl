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

        std::vector<std::string> getTargetPlayer(Monster caster, std::string spell,Fight& f);

        unsigned int orderMonster(Monster m);

        /// @brief Demande au joueur de choisir des actions pour ses monstres
        /// @param f Le combat
        /// @return Les actions choisies par le joueur
        std::vector<Action> getPlayerChoice(Fight& f);

        /// @brief Affiche l'ordre des monstres
        /// @param order L'ordre des monstres
        /// @param f Le combat
        void printOrder(std::vector<Action> order, Fight& f);

        /// @brief Lance le combat
        /// @param f Le combat à lancer
        void fight(Fight& f);


};

#endif