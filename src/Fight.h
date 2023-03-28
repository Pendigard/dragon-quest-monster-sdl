#ifndef _FIGHT
#define _FIGHT

#include <stack>
#include "Monster.h"
#include "jsonFunction.h"

struct Action
{
    std::string idCaster;               // Id du monstre qui lance le sort
    std::vector<std::string> idTargets; // Id des cibles du sort
    std::string spell;                  // Nom du sort
};

enum functionImpact
{
    damage, // Appliquer au monstre la fonction damage
    heal,   // Appliquer au monstre la fonction heal
    stats,  // Appliquer au monstre la fonction d'altération des stats
    status, // Appliquer au monstre la fonction d'altération des status
    null    // Ne rien faire
};

struct spellImpact
{
    std::string message;                     // Message à afficher à l'impact du sort
    functionImpact function;                 // Fonction à appliquer
    std::vector<std::string> targetId;       // Id des cibles
    std::vector<float> argumentFloat;        // Arguments de la fonction
    std::vector<int> argumentInt;            // Arguments de la fonction
    std::vector<std::string> argumentString; // Arguments de la fonction
};

class Fight
{
private:
    /// @brief Indique si le monstre est dans l'équipe 1
    /// @param m : monstre à tester
    /// @return true si le monstre est dans l'équipe 1, false sinon
    bool isTeam1(Monster m) const;

    /// @brief Crée la pile d'actions en fonction de l'agilité des monstres
    /// @param actions : map des id des monstres et de leurs actions
    /// @return la pile d'actions
    void createStackAgility();

    /// @brief Renvoie l'action de la tactique "Sans pitié"
    /// @param caster : monstre qui lance le sort
    /// @return l'action de la tactique "Sans pitié"
    Action tacticSansPitie(Monster caster);

    /// @brief Renvoie l'action de la tactique "Agir avec sagesse"
    /// @param caster : monstre qui lance le sort
    /// @return l'action de la tactique "Agir avec sagesse"
    Action tacticSagesse(Monster caster);

    /// @brief Renvoie l'action de la tactique "Pas de magie"
    /// @param caster : monstre qui lance le sort
    /// @return l'action de la tactique "Pas de magie"
    Action tacticNoMana(Monster caster);

    /// @brief Renvoie l'action de la tactique "Soins avant tout"
    /// @param caster : monstre qui lance le sort
    /// @return l'action de la tactique "Soins avant tout"
    Action tacticSoin(Monster caster);

    /// @brief Vérifie si les cibles du sort sont vivantes
    /// @param targets : cibles du sort
    /// @param spell : nom du sort
    void checkTargetAlive(std::vector<std::string> &targets, std::string spell, Monster caster);

    /// @brief renvoie la phrase correspondant au status
    /// @param status : status à traduire
    /// @return la phrase correspondant au status
    std::string getStatusName(std::string status);

    /// @brief applique les effets du status
    /// @param caster : monstre qui lance le sort
    /// @param canMove : vrai si le monstre peut bouger, faux sinon après l'application du status
    /// @return La file des impacts a appliquer au monstre caster 
    std::queue<spellImpact> getStatusEffect(Monster &caster, bool &canMove);

    /// @brief renvoie la phrase correspondant à la statistique
    /// @param stat : statistique à traduire
    /// @param buff : vrai si c'est un buff, faux si c'est un debuff
    /// @return la phrase correspondant à la statistique
    std::string getStatsName(std::string stat, bool buff);

    /// @brief Simule un sort de dégats
    /// @param caster : monstre qui lance le sort
    /// @param targets : cibles du sort
    /// @param spell : nom du sort
    /// @return la file de message à afficher
    std::queue<spellImpact> simulateAttack(Monster &caster, std::vector<std::string> targets, std::string spell);

    /// @brief Simule un sort de soin
    /// @param caster : monstre qui lance le sort
    /// @param targets : cibles du sort
    /// @param spell : nom du sort
    /// @return la file de message à afficher
    std::queue<spellImpact> simulateHeal(Monster &caster, std::vector<std::string> targets, std::string spell);

    /// @brief consome la mana du monstre
    /// @param caster : monstre qui lance le sort
    /// @param spell : nom du sort
    /// @return true si le monstre a assez de mana, false sinon
    bool consumeMana(Monster &caster, std::string spell);

public:
    unsigned int nbTurn;               // Nombre de tours
    std::vector<Monster> team1;        // Équipe du joueur
    std::vector<Monster> team2;        // Équipe de l'I.A.
    std::vector<Action> actions;       // Actions à effectuer
    std::queue<Action> actionsOrdered; // File des actions à effectuer
    rapidjson::Document spellBase;     // Base de données des sorts
    bool teamFlee;                     // Indique si l'équipe a fui
    bool canScout;                     // Indique si l'équipe peut dresser
    bool scouting;                     // Indique si l'équipe est en train de dresser

    Fight();

    /// @brief Constructeur
    /// @param t1 : équipe du joueur
    /// @param t2 : équipe de l'I.A.
    Fight(std::vector<Monster> t1, std::vector<Monster> t2);

    /// @brief Constructeur par copie
    /// @param fight : fight à copier
    Fight(Fight const &fight);

    Fight &operator=(Fight const &fight);

    /// @brief Choix du sort en fonction de la tactique du monstre
    /// @param caster : monstre qui lance le sort
    void spellTacticChoice(Monster &caster);

    /// @brief Donne une action à effectuer
    void giveActions(std::vector<Action> actions);

    /// @brief Initialise le tour
    /// @return la file de message à afficher
    std::queue<std::string> initTurn();

    /// @brief Simule la prochaine action
    /// @return la file d'impact à effectuer
    std::queue<spellImpact> simulateAction();

    /// @brief Simule le tour
    /// @return la file de message à afficher
    std::queue<std::string> simulateTurn();

    /// @brief Retourne le monstre correspondant à l'id
    /// @param id : id du monstre à trouver
    /// @return le monstre correspondant à l'id
    Monster &getMonsterById(std::string id);

    /// @brief Met à jour les monstres après une action
    /// @param impact : impact de l'action
    void updateMonster(spellImpact impact);

    /// @brief Vérifie si le combat est terminé
    /// @param isTeam1Win : true si l'équipe 1 a gagné, false sinon
    /// @return true si le combat est terminé, false sinon
    bool isOver(bool &isTeam1Win);

    /// @brief Renvoie la liste des monstres vivants de l'équipe
    /// @param team : équipe à tester
    /// @return la liste des ids des monstres vivants de l'équipe
    std::vector<std::string> getTeamAlive(std::vector<Monster> team) const;

    /// @brief Retourne les cibles du sort en fonction de la tactique du monstre
    /// @param caster : monstre qui lance le sort
    /// @param spell : nom du sort
    /// @param isTeam1 : true si le monstre est dans l'équipe 1, false sinon
    /// @return les cibles du sort
    std::vector<std::string> getTargetTactic(Monster caster, std::string spell);

    /// @brief Calcule si l'équipe a fui
    /// @return true si l'équipe a fui, false sinon
    bool flee();

    /// @brief Lance la phase de dressage
    /// @param idMonster : id du monstre à dresser
    /// @return true si le monstre a été dressé, false sinon
    bool scout(std::string idMonster, std::queue<std::string> &messages);

    /// @brief Renvoie l'or gagné
    /// @param db : base de données
    /// @return l'or gagné
    unsigned int getGoldReward(Database &db) const;

    /// @brief Renvoie l'expérience gagnée
    /// @param db : base de données
    /// @return l'expérience gagnée
    unsigned int getExpReward(Database &db) const;
};
/// @brief Crée une action
/// @param idCaster id du monstre qui lance le sort
/// @param spell nom du sort
/// @param idTargets id des cibles du sort
/// @return l'action créée
Action createAction(std::string idCaster, std::string spell, std::vector<std::string> idTargets);

#endif