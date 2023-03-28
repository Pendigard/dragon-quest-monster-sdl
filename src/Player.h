#ifndef _PLAYER
#define _PLAYER

#include <unordered_map>
#include <string>
#include <vector>
#include "Monster.h"

class Player
{
private:
    unsigned int gold; // Or du joueur
public:
    std::vector<Monster> mainTeam;        // Equipe principale
    std::vector<Monster> benchTeam;       // Equipe de remplacement
    std::vector<Monster> storageMonsters; // Monstres en stock

    /// @brief Ajoute un monstre à l'équipe
    /// @param m Le monstre à ajouter
    /// @param team L'équipe dans laquelle ajouter le monstre
    void addMonster(const Monster &m, unsigned int team);

    /// @brief Supprime un monstre de l'équipe
    /// @param index L'indice du monstre à supprimer
    /// @param team L'équipe dans laquelle supprimer le monstre
    void removeMonster(const int index, unsigned int team);

    /// @brief Ajoute de l'or au joueur
    /// @param g L'or à ajouter
    void addGold(const unsigned int g);

    /// @brief Retire de l'or au joueur
    /// @param g L'or à retirer
    void removeGold(const unsigned int g);

    /// @brief Retourne l'or du joueur
    /// @return L'or du joueur
    unsigned int getGold() const;
};

#endif