#ifndef _MONSTER
#define _MONSTER



#include "jsonFunction.h"

class Monster
{
private:
    float hp;
    float mp;
    unsigned int exp;
    unsigned int level;
    unsigned int synthLevel;
    unsigned int skillPoints;
    std::vector<std::string> spells;
    std::unordered_map<std::string, std::string> infos;
    std::unordered_map<std::string, float> stats;
    std::unordered_map<std::string, float> resistances;
    std::unordered_map<std::string, float> growth;
    std::unordered_map<std::string, unsigned int> skills;
    std::unordered_map<std::string, float[2]> alterations;
    std::unordered_map<std::string, int> status;
    
    ///@brief Met à jour le niveau du monstre avec les points d'expérience
    void getLevelXp();

    ///@brief Augmente le niveau du monstre et met à jour ses statistiques
    ///@param level : niveau du monstre
    void levelUp(unsigned int level);

    ///@brief Met à jour les sorts du monstre en fonction de ses compétences
    ///@param skillBase : base de données des compétences
    void updateSpell(rapidjson::Document& skillBase);

    ///@brief Initialise les status du monstre
    void initStatus();

    std::string generateID() const;

public:
    ///@brief Constructeur à partir d'un fichier json
    ///@param monsterData : données du monstre, idM : identifiant du monstre, monsterBase : base de données des monstres, skillBase : base de données des compétences
    Monster(rapidjson::Value& monsterData,std::string idM,rapidjson::Document& monsterBase,rapidjson::Document& skillBase);

    ///@brief Constructeur à partir des statistiques du monstre
    Monster(std::string name,std::string type,rapidjson::Document& monsterBase,rapidjson::Document& skillBase,rapidjson::Document& save);

    ///@brief Affiche les caractéristiques du monstre dans la console
    void print() const;

    ///@brief Affiche les sorts du monstre dans la console
    void printSpells() const;

    ///@brief Ajoute des points d'expérience au monstre
    ///@param xp : nombre de points d'expérience à ajouter
    void addXp(unsigned int xp);

    ///@brief Ajoute des points de compétence au monstre
    ///@param points : nombre de points de compétence à ajouter
    void addSkillPoint(unsigned int points);

    /// @brief Applique les points de compétence au monstre dans un de ses sets de compétences
    /// @param points, skill, skillBase : points de compétence à appliquer, nom de la compétence, base de données des compétences
    void applySkillPoint(unsigned int points,std::string skill,rapidjson::Document& skillBase);

    /// @brief Retourne l'identifiant du monstre
    /// @return l'identifiant du monstre
    std::string getId() const;

    /// @brief Met à jour les statistiques du monstre dans la sauvegarde
    /// @param m, save : monstre, sauvegarde
    void updateSaveMonster(rapidjson::Document& save) const;

    void createSaveMonster(rapidjson::Document& save) const;
};

#endif