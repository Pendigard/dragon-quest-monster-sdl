#ifndef _MONSTER
#define _MONSTER

#include <queue>

#include "jsonFunction.h"
#include "Database.h"

enum Tactics
{
    nullTactic = -1,
    SansPitie = 0, // Sans pitié
    Soin = 1,      // Soin avant tout
    Sagesse = 2,   // Agir avec sagesse
    NoMana = 3     // Pas de magies
};

class Monster
{
private:
    unsigned int exp;                                      // Experience totale du monstre
    unsigned int level;                                    // Niveau du monstre
    unsigned int synthLevel;                               // Niveau de synthèse du monstre
    unsigned int skillPoints;                              // Points de compétence du monstre
    unsigned int synthId;                                  // Identifiant de synthèse du monstre
    Tactics tactic;                                        // Tactique du monstre
    std::unordered_map<std::string, float> stats;          // Statistiques du monstre
    std::unordered_map<std::string, unsigned int> statMax; // Statistiques maximales du monstre
    std::unordered_map<std::string, float> resistances;    // Résistances du monstre
    std::unordered_map<std::string, float> growth;         // Croissance des statistiques du monstre
    std::unordered_map<std::string, float> alterations;    // Statistiques modifiées par des effets
    std::unordered_map<std::string, int> alterationsTurn;  // Nombre de tours restants des effets
    std::unordered_map<std::string, int> status;           // Status du monstre
    std::unordered_map<std::string, unsigned int> skills;  // Compétences du monstre
    std::vector<std::string> spells;                       // Sorts du monstre
    std::unordered_map<std::string, std::string> infos;    // Informations du monstre

    ///@brief Augmente le niveau du monstre et met à jour ses statistiques
    void levelUp();

    ///@brief Initialise les status du monstre
    void initStatus();

    /// @brief Génère un identifiant unique pour le monstre
    /// @return L'identifiant unique
    std::string generateID() const;

    /// @brief Renvoie l'xp maximum que le monstre peut atteindre
    /// @return L'xp maximum
    int getMaxLvl() const;

public:
    float hp; // Points de vie du monstre
    float mp; // Points de mana du monstre
    
    ///@brief Constructeur à partir d'un monstre dans la sauvegarde
    ///@param monsterData données du monstre
    ///@param idM identifiant du monstre
    ///@param database base de données
    Monster(rapidjson::Value &monsterData, std::string idM, Database &database);

    ///@brief Constructeur de monstre sauvage
    Monster(std::string name, std::string type, Database &database, rapidjson::Document &save, unsigned int lvl);

    /// @brief Constructeur de monstre de synthèse
    /// @param m1 Monstre parent 1
    /// @param m2 Monstre parent 2
    /// @param database Base de données
    /// @param skills Compétences du monstre
    Monster(Monster &m1, Monster &m2, std::string name, std::string type, Database &database, std::unordered_map<std::string, unsigned int> &skills, rapidjson::Document &save);

    /// @brief génère un monstre null
    Monster();

    ~Monster();

    ///@brief Affiche les caractéristiques du monstre dans la console
    void print() const;

    ///@brief Affiche les sorts du monstre dans la console
    void printSpells() const;

    ///@brief Ajoute des points d'expérience au monstre
    ///@param xp nombre de points d'expérience à ajouter
    ///@return vrai si le monstre a gagné un niveau
    bool addXp(unsigned int xp);

    ///@brief Ajoute des points de compétence au monstre
    ///@param points nombre de points de compétence à ajouter
    void addSkillPoint(unsigned int points);

    /// @brief Applique les points de compétence au monstre dans un de ses sets de compétences
    /// @param points nombre de points de compétence à appliquer
    ///@param skill set de compétence dans lequel appliquer les points
    ///@param skillBase base de données des compétences
    void applySkillPoint(unsigned int points, std::string skill, Database &db);

    /// @brief Met à jour les statistiques du monstre dans la sauvegarde
    /// @param save sauvegarde
    void updateSaveMonster(rapidjson::Document &save) const;

    /// @brief Crée un monstre dans la sauvegarde
    /// @param save sauvegarde
    void createSaveMonster(rapidjson::Document &save) const;

    /// @brief vrai si les deux monstres ont le même identifiant
    /// @param m monstre à comparer
    /// @return vrai si les deux monstres ont le même identifiant
    bool operator==(const Monster &m) const;

    /// @brief Vrai si les deux monstres n'ont pas le même identifiant
    /// @param m Monstre à comparer
    /// @return Vrai si les deux monstres n'ont pas le même identifiant
    bool operator!=(const Monster &m) const;

    /// @brief Donne l'agilité du monstre
    /// @return Retourne l'agilité du monstre
    float getAgility() const;

    /// @brief Donne l'experience du monstre
    /// @return Retourne l'experience du monstre
    unsigned int getExp() const;

    /// @brief Accesseur du niveau du monstre
    /// @return Retourne le niveau du monstre
    unsigned int getLevel() const;

    /// @brief Donne l'xp nécessaire pour atteindre le niveau demandé
    /// @param level niveau demandé
    /// @return Retourne l'xp nécessaire pour atteindre le niveau demandé
    int getXpLvl(int level) const;

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
    /// @param stat statistique demandée
    /// @return Retourne la statistique du monstre demandée
    float getStat(std::string stat) const;

    /// @brief Donne les resistances du monstre
    /// @param resistance resistance demandée
    /// @return Retourne la resistance du monstre demandée
    float getResistance(std::string resistance) const;

    /// @brief Donne la tactique du monstre
    /// @return Retourne la tactique du monstre
    Tactics getTactic() const;

    /// @brief Définit la tactique du monstre
    /// @param tactic tactique du monstre
    void setTactic(Tactics tactic);

    /// @brief Inflige des dégâts au monstre
    /// @param damage dégâts à infliger
    void damage(float damage);

    /// @brief Soigne le monstre
    /// @param heal points de vie à soigner
    void heal(float heal);

    /// @brief Défini les alterations de statistiques du monstre
    /// @param alteration statistique à modifier
    /// @param value valeur de l'alteration
    /// @param turn nombre de tours de l'alteration
    void setStats(std::string alteration, float value, int turn);

    /// @brief Défini les alterations de status du monstre
    /// @param status status à modifier
    /// @param turn nombre de tours du status
    void setStatus(std::string status, int turn);

    /// @brief Met à jour les alterations de statistiques et de status du monstre
    /// @return Retourne la liste des status qui ont été supprimés
    std::queue<std::string> updateStatStatus();

    /// @brief Renvoie vrai si le status du monstre est altéré
    /// @param status status à vérifier
    /// @return Retourne vrai si le status du monstre est altéré
    bool getStatus(std::string status) const;

    /// @brief retourne une informations du monstre
    /// @param info information demandée
    /// @return Retourne l'informations du nom info du monstre
    std::string getInfos(std::string info) const;

    /// @brief Donne l'identifiant de synthèse du monstre
    /// @return Retourne l'identifiant de synthèse du monstre
    unsigned int getSynthId() const;

    /// @brief Donne les points de compétence du monstre qui n'ont pas encore été appliqués
    /// @return Retourne les points de compétence du monstre qui n'ont pas encore été appliqués
    unsigned int getSkillToAttribute() const;

    /// @brief Attribution automatique des points de compétence du monstre
    /// @param db base de données
    void autoAttributeSkill(Database &db);

    /// @brief change le nom du monstre
    /// @param name nouveau nom du monstre
    void setName(std::string name);

    /// @brief Soigne le monstre à 100%
    void fullHeal();

    /// @brief Donne les compétences du monstre
    /// @return Retourne les compétences du monstre
    std::unordered_map<std::string, unsigned int> getSkills() const;

    /// @brief Donne le niveau de synthèse du monstre
    /// @return Retourne le niveau de synthèse du monstre
    unsigned int getSynthLevel() const;

    /// @brief Donne la croissance du monstre
    /// @return Retourne la croissance du monstre
    std::unordered_map<std::string, float> getGrowth() const;

    /// @brief Retourne les hp du monstre
    /// @return Retourne les hp du monstre
    unsigned int getHp() const;

    /// @brief Retourne les mp du monstre
    /// @return Retourne les mp du monstre
    unsigned int getMp() const;
};

#endif