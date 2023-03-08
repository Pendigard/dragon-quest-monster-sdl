#ifndef _FIGHT
#define _FIGHT

#include <stack>
#include <queue>
#include "Monster.h"
#include "jsonFunction.h"

struct Action
{
    std::string idCaster;
    std::vector<std::string> idTargets;
    std::string spell;
};

enum functionImpact
{
    damage,
    heal,
    stat,
    status,
    null
};

struct spellImpact
{
    std::string message;
    functionImpact function=null;
    std::vector<std::string> targetId;
    std::vector<float> argumentFloat;
    std::vector<int> argumentInt;
    std::vector<std::string> argumentString;
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

    /// @brief Retourne les cibles du sort en fonction de la tactique du monstre
    /// @param caster : monstre qui lance le sort
    /// @param spell : nom du sort
    /// @param isTeam1 : true si le monstre est dans l'équipe 1, false sinon
    /// @return les cibles du sort
    std::vector<std::string> getTargetTactic(Monster caster, std::string spell, bool isTeam1);




public:
    unsigned int nbTurn;
    std::vector<Monster> team1;
    std::vector<Monster> team2;
    std::vector<Action> actions;
    std::queue<Action> actionsOrdered;
    rapidjson::Document spellBase;

    Fight(std::vector<Monster> t1, std::vector<Monster> t2);

    /// @brief Choix du sort en fonction de la tactique du monstre
    /// @param caster : monstre qui lance le sort
    /// @param isTeam1 : true si le monstre est dans l'équipe 1, false sinon
    void spellTacticChoice(Monster& caster, bool isTeam1);


    /// @brief Donne une action à effectuer
    void giveActions(std::vector<Action> actions);

    void initTurn();
    /// @brief Simule une action
    /// @return la file de message à afficher
    std::queue<spellImpact> simulateAction();
    
    /// @brief Retourne le monstre correspondant à l'id
    /// @param id : id du monstre à trouver
    /// @return le monstre correspondant à l'id
    Monster& getMonsterById(std::string id);

    void updateMonster(spellImpact impact);

};
/// @brief Crée une action
/// @param idCaster id du monstre qui lance le sort
/// @param spell nom du sort
/// @param idTargets id des cibles du sort
/// @return l'action créée
Action createAction(std::string idCaster, std::string spell, std::vector<std::string> idTargets);

#endif