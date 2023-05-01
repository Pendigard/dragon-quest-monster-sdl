#ifndef _DATABASE
#define _DATABASE
#include <vector>
#include "jsonFunction.h"
class Database
{
private:
    rapidjson::Document monsterBase;                               // Base de données des monstres
    rapidjson::Document library;                                   // Base de données de la librairie des monstres
    rapidjson::Document skillBase;                                 // Base de données des compétences
    rapidjson::Document spellBase;                                 // Base de données des sorts
    rapidjson::Document synthesisRecipe;                           // Base de données des recettes de synthese
    rapidjson::Document scripts;                                   // Base de données des scripts
    std::unordered_map<std::string, rapidjson::Document> tilesets; // Clé : nom du fichier du tileset, Valeur : contenu du tileset
public:
    /// @brief Constructeur lance la fonction loadDatabase()
    Database();

    /// @brief Charge les données de monster.json, library.json, skill.json et spell.json
    void loadDatabase();

    /// @brief retourne la base de données des monstres
    /// @return La base de données des monstres
    rapidjson::Document &getMonsterBase();

    /// @brief retourne les statistiques d'un monstre à partir de son type
    /// @param type Le type du monstre
    /// @return Les statistiques du monstre
    rapidjson::Value &getMonsterStatType(std::string type);

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

    /// @brief retourne la largeur du tileset
    /// @param tileset Le nom du fichier du tileset
    /// @return La largeur du tileset
    int getTileSetWidth(std::string tileset) const;

    /// @brief retourne la hauteur du tileset
    /// @param tileset Le nom du fichier du tileset
    /// @return La hauteur du tileset
    int getTileSetHeight(std::string tileset) const;

    /// @brief retourne la largeur d'une tile d'un tileset
    /// @param tileset Le nom du fichier du tileset
    /// @return La largeur d'une tile d'un tileset
    int getTileWidth(std::string tileset) const;

    /// @brief retourne la hauteur d'une tile d'un tileset
    /// @param tileset Le nom du fichier du tileset
    /// @return La hauteur d'une tile d'un tileset
    int getTileHeight(std::string tileset) const;

    /// @brief retourne le chemin de fichier vers l'image associé au tileset
    /// @param tileset Le nom du fichier du tileset
    /// @return Le chemin de fichier vers l'image associé au tileset
    std::string getImage(std::string tileset) const;

    /// @brief retourne un script
    /// @param scriptName Le nom du script
    /// @return Le script
    rapidjson::Value &getScript(std::string scriptName);
};
#endif