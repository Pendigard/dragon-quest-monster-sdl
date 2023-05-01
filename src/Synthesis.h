#ifndef SYNTHESIS_H
#define SYNTHESIS_H

#include "Monster.h"

/// @brief Retourne le type des monstres qui peuvent être résultat de la synthese
/// @param parent1 Monstre parent 1
/// @param parent2 Monstre parent 2
/// @param db Base de données
/// @return Retourne le type des monstres qui peuvent être résultat de la synthese
std::vector<std::string> makeSynthesisPreview(Monster parent1, Monster parent2, Database &db);

/// @brief Vrai si une recette existe entre les deux monstres
/// @param parent1 Monstre parent 1
/// @param parent2 Monstre parent 2
/// @param db Base de données
/// @return Vrai si une recette existe entre les deux monstres
bool isRecipe(Monster parent1, Monster parent2, Database &db);

/// @brief Retourne le nom du monstre résultat de la synthese recette
/// @param parent1 Monstre parent 1
/// @param parent2 Monstre parent 2
/// @param db Base de données
/// @return Retourne le nom du monstre résultat de la synthese recette
std::string getMonsterFromRecipe(Monster parent1, Monster parent2, Database &db);

/// @brief Retourne le rang du monstre résultat de la synthese
/// @param parent1 Monstre parent 1
/// @param parent2 Monstre parent 2
/// @return Retourne le rang du monstre résultat de la synthese
std::string getSynthesisRank(Monster parent1, Monster parent2);

/// @brief Retourne le nom du monstre résultat pour chaque parent de la synthese en fonction du rang le plus élevé
/// @param parent1 Monstre parent 1
/// @param rank Rang du monstre résultat
/// @return Retourne le nom du monstre résultat pour chaque parent de la synthese en fonction du rang le plus élevé
std::string getChildMonster(Monster parent1, std::string rank);

/// @brief Retourne les compétences selectionnable pour la synthese
/// @param parent1 Monstre parent 1
/// @param parent2 Monstre parent 2
/// @param child Monstre résultat
/// @param db Base de données
/// @return Retourne les compétences selectionnable pour la synthese
std::unordered_map<std::string, unsigned int> getSynthesisSkills(Monster parent1, Monster parent2, std::string child, Database &db);

#endif