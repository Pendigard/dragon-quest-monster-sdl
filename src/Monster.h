#ifndef _MONSTER
#define _MONSTER


#include <queue>

#include "jsonFunction.h"



class Monster
{
private:
    unsigned int exp;
    unsigned int level;
    unsigned int synthLevel;
    unsigned int skillPoints;
    unsigned int synthId;
    std::string tactic;
    std::unordered_map<std::string, float> stats;
    std::unordered_map<std::string,unsigned int> statMax;
    std::unordered_map<std::string, float> resistances;
    std::unordered_map<std::string, float> growth;
    std::unordered_map<std::string, unsigned int> skills;
    std::unordered_map<std::string, float> alterations;
    std::unordered_map<std::string, int> alterationsTurn;
    std::unordered_map<std::string, int> status;
    std::vector<std::string> spells;
    std::unordered_map<std::string, std::string> infos;
    
    ///@brief Met à jour le niveau du monstre avec les points d'expérience
    void getLevelXp();

    ///@brief Augmente le niveau du monstre et met à jour ses statistiques
    ///@param level : niveau du monstre
    void levelUp(unsigned int level);

    ///@brief Initialise les status du monstre
    void initStatus();

    /// @brief Génère un identifiant unique pour le monstre
    /// @return L'identifiant unique
    std::string generateID() const;

    /// @brief Renvoie l'xp maximum que le monstre peut atteindre
    /// @return L'xp maximum
    int getMaxXp() const;

public:

    float hp;
    float mp;
    ///@brief Constructeur à partir d'un fichier json
    ///@param monsterData : données du monstre, idM : identifiant du monstre, monsterBase : base de données des monstres, skillBase : base de données des compétences
    Monster(rapidjson::Value& monsterData,std::string idM,rapidjson::Document& monsterBase,rapidjson::Document& skillBase,rapidjson::Document& library);

    ///@brief Constructeur à partir des statistiques du monstre
    Monster(std::string name,std::string type,rapidjson::Document& monsterBase,rapidjson::Document& skillBase,rapidjson::Document& save,rapidjson::Document& library,unsigned int lvl);

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

    /// @brief Donne la statistique du monstre demandée
    /// @param stat : statistique demandée
    /// @return Retourne la statistique du monstre demandée
    float getStat(std::string stat) const;

    /// @brief Donne les resistances du monstre
    /// @param resistance : resistance demandée
    /// @return Retourne la resistance du monstre demandée
    float getResistance(std::string resistance) const;

    /// @brief Donne la tactique du monstre
    /// @return Retourne la tactique du monstre
    std::string getTactic() const;

    /// @brief Définit la tactique du monstre
    /// @param tactic : tactique du monstre
    void setTactic(std::string tactic);

    /// @brief Inflige des dégâts au monstre
    /// @param damage : dégâts à infliger
    void damage(float damage);

    /// @brief Soigne le monstre
    /// @param heal : points de vie à soigner
    void heal(float heal);

    /// @brief Défini les alterations de statistiques du monstre
    /// @param alteration : statistique à modifier
    /// @param value : valeur de l'alteration
    /// @param turn : nombre de tours de l'alteration
    void setStats(std::string alteration, float value, int turn);

    /// @brief Défini les alterations de status du monstre
    /// @param status : status à modifier
    /// @param turn : nombre de tours du status
    void setStatus(std::string status, int turn);

    /// @brief Met à jour les alterations de statistiques et de status du monstre
    /// @return Retourne la liste des status qui ont été supprimés
    std::queue<std::string> updateStatStatus();

    /// @brief Renvoie vrai si le status du monstre est altéré
    /// @param status : status à vérifier
    /// @return Retourne vrai si le status du monstre est altéré
    bool getStatus(std::string status) const;

    std::string getInfos(std::string info) const;

    unsigned int getSynthId() const;

    unsigned int getSkillToAttribute() const;

    void autoAttributeSkill(rapidjson::Document& skillBase);

    void printSkill() const;

};

#endif