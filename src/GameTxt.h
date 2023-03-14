#ifndef _GAME
#define _GAME

#include "Player.h"
#include "jsonFunction.h"
#include "Fight.h"
#include "Synthesis.h"
#include <unistd.h>

class Game
{
    public:
        Player player;
        rapidjson::Document monsterBase;
        rapidjson::Document library;
        rapidjson::Document save;
        rapidjson::Document skillBase;
        rapidjson::Document spellBase;
        
        /// @brief  Charge les données de save.json, skill.json, spell.json et monster.json
        void loadGame();

        /// @brief  Sauvegarde les données dans save.json, skill.json, spell.json et monster.json
        void saveGame();

        /// @brief Demande au joueur de choisir une cible
        /// @param isOffensive Si le sort est offensif ou non
        /// @param f Le combat
        /// @return L'indice de la cible choisie
        unsigned int askTarget(bool isOffensive,Fight& f);

        /// @brief Demande les cibles possibles pour un sort
        /// @param caster Le monstre qui lance le sort
        /// @param spell Le sort
        /// @param f Le combat
        /// @return Les cibles possibles
        std::vector<std::string> getTargetPlayer(Monster caster, std::string spell,Fight& f);

        /// @brief Demande au joueur de donner un ordre à ses monstres
        /// @param m Le monstre à ordonner
        /// @return L'ordre du monstre
        unsigned int orderMonster(Monster m);

        /// @brief Demande au joueur de choisir des actions pour ses monstres
        /// @param f Le combat
        /// @return Les actions choisies par le joueur
        std::vector<Action> getPlayerChoice(Fight& f);

        /// @brief Demande au joueur de choisir une tactique
        /// @param f Le combat
        void getTacticChoice(Fight& f);

        /// @brief Affiche l'ordre des monstres
        /// @param order L'ordre des monstres
        /// @param f Le combat
        void printOrder(std::vector<Action> order, Fight& f);

        /// @brief Lance le combat
        /// @param f Le combat à lancer
        void fight(Fight& f);

        void deleteFromSave(std::string id);


};

#endif