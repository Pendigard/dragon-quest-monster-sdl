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
    std::string tactic;
    std::unordered_map<std::string,unsigned int> statMax;
    std::unordered_map<std::string, float> stats;
    std::unordered_map<std::string, float> resistances;
    std::unordered_map<std::string, float> growth;
    std::unordered_map<std::string, unsigned int> skills;
    std::unordered_map<std::string, float[2]> alterations;
    std::unordered_map<std::string, int> status;
    std::vector<std::string> spells;
    std::unordered_map<std::string, std::string> infos;
    
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

    int getMaxXp() const;

public:

    Monster();
    ///@brief Constructeur à partir d'un fichier json
    ///@param monsterData : données du monstre, idM : identifiant du monstre, monsterBase : base de données des monstres, skillBase : base de données des compétences
    Monster(rapidjson::Value& monsterData,std::string idM,rapidjson::Document& monsterBase,rapidjson::Document& skillBase);

    ///@brief Constructeur à partir des statistiques du monstre
    Monster(std::string name,std::string type,rapidjson::Document& monsterBase,rapidjson::Document& skillBase,rapidjson::Document& save,unsigned int lvl);

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
    /// @param points nombre de points de compétence à appliquer
    ///@param skill set de compétence dans lequel appliquer les points
    ///@param skillBase base de données des compétences
    void applySkillPoint(unsigned int points,std::string skill,rapidjson::Document& skillBase);

    /// @brief Met à jour les statistiques du monstre dans la sauvegarde
    /// @param save : sauvegarde
    void updateSaveMonster(rapidjson::Document& save) const;

    /// @brief Crée un monstre dans la sauvegarde
    /// @param save : sauvegarde
    void createSaveMonster(rapidjson::Document& save) const;

    /// @brief vrai si les deux monstres ont le même identifiant
    /// @param m : monstre à comparer
    /// @return vrai si les deux monstres ont le même identifiant
    bool operator==(const Monster& m) const;

        /// @brief Retourne l'identifiant du monstre
    /// @return l'identifiant du monstre
    std::string getId() const;

    /// @brief Donne l'agilité du monstre
    /// @return Retourne l'agilité du monstre
    float getAgility() const;

    /// @brief Donne l'experience du monstre
    /// @return Retourne l'experience du monstre
    unsigned int getExp() const;

    /// @brief Donne les sorts du monstre
    /// @return Retourne les sorts du monstre
    std::vector<std::string> getSpells() const;

    /// @brief Donne le nom du monstre
    /// @return Retourne le nom du monstre
    std::string getName() const;

    /// @brief Donne le type du monstre
    /// @return Retourne le type du monstre
    std::string getType() const;
};

#endif