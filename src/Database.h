#ifndef _DATABASE
#define _DATABASE
#include <vector>
#include "jsonFunction.h"
class Database
{
private:
    rapidjson::Document monsterBase;     // Base de données des monstres
    rapidjson::Document library;         // Base de données de la librairie des monstres
    rapidjson::Document skillBase;       // Base de données des compétences
    rapidjson::Document spellBase;       // Base de données des sorts
    rapidjson::Document synthesisRecipe; // Base de données des recettes de synthese
public:
    /// @brief Constructeur lance la fonction loadDatabase()
    Database();
    Database& operator=(const Database& other);
    /// @brief Charge les données de monster.json, library.json, skill.json et spell.json
    void loadDatabase();
    /// @brief retourne la base de données des monstres
    /// @return La base de données des monstres
    rapidjson::Document &getMonsterBase();
    /// @brief retourne la base de données de la librairie des monstres
    /// @return La base de données de la librairie des monstres
    rapidjson::Document &getLibrary();
    /// @brief retourne la base de données des compétences
    /// @return La base de données des compétences
    rapidjson::Document &getSkillBase();
    /// @brief retourne la base de données des sorts
    /// @return La base de données des sorts
    rapidjson::Document &getSpellBase();
    /// @brief retourne les statistiques d'un monstre
    /// @param family La famille du monstre
    /// @param rank Le rang du monstre
    /// @param type Le type du monstre
    /// @return Les statistiques du monstre
    rapidjson::Value &getMonsterStat(std::string family, std::string rank, std::string type);
    /// @brief Retourne la base de données des recettes de synthese
    /// @return La base de données des recettes de synthese
    rapidjson::Document &getSynthesisRecipe();
};
#endif