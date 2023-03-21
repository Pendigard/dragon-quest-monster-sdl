#ifndef _GAME
#define _GAME

#include "Player.h"
#include "jsonFunction.h"
#include "Fight.h"
#include "Synthesis.h"
#include <unistd.h>

class Game
{
public:
    Player player;            // Le joueur
    rapidjson::Document save; // Les données de sauvegarde
    Database database;        // La base de données

    /// @brief  Charge les données de save.json, skill.json, spell.json et monster.json
    void loadGame();

    /// @brief  Sauvegarde les données dans save.json, skill.json, spell.json et monster.json
    void saveGame();

    /// @brief Demande au joueur de choisir une cible
    /// @param isOffensive Si le sort est offensif ou non
    /// @param f Le combat
    /// @return L'indice de la cible choisie
    unsigned int askTarget(bool isOffensive, Fight &f);

    /// @brief Demande les cibles possibles pour un sort
    /// @param caster Le monstre qui lance le sort
    /// @param spell Le sort lancé
    /// @param f Le combat
    /// @return Les cibles possibles
    std::vector<std::string> getTargetPlayer(Monster caster, std::string spell, Fight &f);

    /// @brief Demande au joueur de donner un ordre à ses monstres
    /// @param m Le monstre à ordonner
    /// @return L'ordre du monstre
    unsigned int orderMonster(Monster m);

    /// @brief Demande au joueur de choisir des actions pour ses monstres
    /// @param f Le combat
    /// @return Les actions choisies par le joueur
    std::vector<Action> getPlayerChoice(Fight &f);

    /// @brief Demande au joueur de choisir une tactique
    /// @param f Le combat
    void getTacticChoice(Fight &f);

    /// @brief Affiche les ordres donnés par le joueur à ses monstres
    /// @param order Les ordres donnés par le joueur
    /// @param f Le combat
    void printOrder(std::vector<Action> order, Fight &f);

    /// @brief Lance le combat
    /// @param f Le combat à lancer
    /// @return Si l'équipe du joueur est encore en vie
    bool fight(Fight &f);

    /// @brief Supprime un monstre de la sauvegarde
    /// @param id L'identifiant du monstre à supprimer
    void deleteFromSave(std::string id);

    /// @brief Crée une équipe de monstres sauvages
    /// @param monsters Les monstres possiblement présents dans l'équipe
    /// @param levelMin Niveau minimum des monstres
    /// @param levelMax Niveau maximum des monstres
    /// @return L'équipe de monstres sauvages
    std::vector<Monster> createWildMonsterTeam(std::vector<std::string> monsters, unsigned int levelMin, unsigned int levelMax, std::string monster);

    /// @brief Demande au joueur quoi faire avec le monstre nouvellement capturé
    /// @param id L'identifiant du monstre capturé
    void scoutMonster(std::string id, Fight &f);

    /// @brief Lance le jeu en mode texte
    void play();

    /// @brief Affiche le menu du centre des dresseurs
    void scoutCenter();

    /// @brief Soigne l'équipe du joueur
    void healTeam();

    /// @brief Affiche le menu d'organisation des monstres du joueur
    void organizeMonsters();

    /// @brief Simule une exploration
    /// @param monsterZone Les monstres qui peuvent être rencontrés
    /// @param niveauMin Niveau minimum des monstres rencontrés
    /// @param niveauMax Niveau maximum des monstres rencontrés
    void exploration(std::vector<std::string> monsterZone, int niveauMin, int niveauMax);

    /// @brief Récompense le joueur après un combat
    /// @param f Le combat
    void getReward(Fight &f);

    /// @brief Affiche le menu d'attribution des points de compétences
    /// @param m Le monstre à attribuer des points
    void attributePoint(Monster &m);

    /// @brief Affiche le menu de synthèse et lance la synthèse choisie
    void synthesis();

    /// @brief donne les monstres qui peuvent être synthétisés en fonction de la charge et du premier choix
    /// @param charge La charge du monstre
    /// @param firstChoice Le premier choix du joueur
    /// @return Les monstres qui peuvent être synthétisés
    std::vector<Monster> getSynthetisableMonster(std::string charge, std::string firstChoice);

    /// @brief Demande au joueur de choisir les sets de compétences pour la synthèse au joueur
    std::unordered_map<std::string, unsigned int> getSkillSynthChoice(Monster m1, Monster m2, std::string child);

    /// @brief supprime un monstre de la sauvegarde
    /// @param id L'identifiant du monstre à supprimer
    void deleteMonster(std::string id);
};

#endif