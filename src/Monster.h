#ifndef _MONSTER
#define _MONSTER


#include <cmath>
#include <fstream>
#include <string>
#include <iostream>
#include <unordered_map>
#include <string>
#include <rapidjson/document.h>

class Monster
{
    private:
        float hp;
        float mp;
        unsigned int exp;
        unsigned int level;
        unsigned int synthLevel;
        std::unordered_map<std::string, std::string> infos;
        std::unordered_map<std::string, float> stats;
        std::unordered_map<std::string, float> resistances;
        std::unordered_map<std::string, float> growth;
        std::unordered_map<std::string, unsigned int> skills;
        /*
        std::unordered_map<std::string, float> alterations;
        std::unordered_map<std::string, float> status;
        */

       void getLevelXp();
       void levelUp(unsigned int level);
       void getSpell(rapidjson::Document& spellBase, rapidjson::Document& skillBase);

    public:
        ///@brief Constructeur à partir d'un fichier json
        Monster(rapidjson::Value& monsterData,std::string idM,rapidjson::Document& monsterBase);

        ///@brief Affiche les caractéristiques du monstre dans la console
        void afficher() const;


       void addXp(unsigned int xp);
};

#endif