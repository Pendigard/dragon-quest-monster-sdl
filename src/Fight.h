#ifndef _FIGHT
#define _FIGHT

#include <stack>
#include "Monster.h"
#include "jsonFunction.h"

struct Action
{
    std::string idCaster;
    std::vector<Monster> idTargets;
    std::string spell;
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
    std::vector<Monster> getTargetTactic(Monster caster, std::string spell, bool isTeam1);

    /// @brief Retourne les cibles du sort en fonction du choix du joueur
    /// @param caster : monstre qui lance le sort
    /// @param spell : nom du sort
    /// @return les cibles du sort
    std::vector<Monster> getTargetPlayer(Monster caster, std::string spell);

public:
    unsigned int nbTurn;
    std::vector<Monster> team1;
    std::vector<Monster> team2;
    std::vector<Action> actions;
    std::stack<Action> actionsOrdered;
    rapidjson::Document spellBase;

    Fight(std::vector<Monster> t1, std::vector<Monster> t2);

    /// @brief Choix du sort en fonction de la tactique du monstre
    /// @param caster : monstre qui lance le sort
    /// @param isTeam1 : true si le monstre est dans l'équipe 1, false sinon
    /// @return l'action du monstre (sort, cible, etc.)
    Action spellTacticChoice(Monster& caster, bool isTeam1);


    /// @brief Donne une action à effectuer
    void giveActions(std::vector<Action> actions);

    void initTurn();

    std::string simulateAction();
    
    /// @brief Retourne le monstre correspondant à l'id
    /// @param id : id du monstre à trouver
    /// @return le monstre correspondant à l'id
    Monster getMonsterById(std::string id);

};

Action createAction(std::string idCaster, std::string spell, std::vector<Monster> idTargets);

#endif