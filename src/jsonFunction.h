#ifndef JSONFUNCTION_H
#define JSONFUNCTION_H

#include <fstream>
#include <iostream>

#include <unordered_map>
#include <vector>
#include <string>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <cstdio>

#include "../extern/rapidjson-master/include/rapidjson/document.h"
#include "../extern/rapidjson-master/include/rapidjson/writer.h"
#include "../extern/rapidjson-master/include/rapidjson/stringbuffer.h"

/// @brief Convertit un json en unordered_map
/// @param json, map : json à convertir, map dans laquelle on stocke les données
void jsonToUnorderedMap(rapidjson::Value& json, std::unordered_map<std::string, unsigned int>& map);

/// @brief Convertit un json en unordered_map
/// @param json, map : json à convertir, map dans laquelle on stocke les données
void jsonToUnorderedMap(rapidjson::Value& json, std::unordered_map<std::string, float>& map);

/// @brief Convertit une liste d'un fichier json en std::vector
/// @param jsonList : La liste du fichier json
/// @param vec : Le vecteur dans lequel on stocke les données
void jsonToVector(rapidjson::Value& jsonList, std::vector<std::string>& vec);

/// @brief Charge un fichier json
/// @param path, doc : chemin du fichier, document dans lequel on stocke les données
void loadJson(std::string path, rapidjson::Document& doc);

/// @brief Sauvegarde un fichier json
/// @param path : chemin du fichier
/// @param doc : document à sauvegarder
void saveJson(std::string path, rapidjson::Document& doc);

/// @brief Formate une chaîne de caractères
/// @param fmt_str : chaîne de caractères à formater
/// @param args : arguments à insérer dans la chaîne de caractères
/// @return chaîne de caractères formatée
std::string formatString(const std::string fmt_str,std::vector<std::string> args);

/// @brief Crée une chaîne de caractères contenant des espaces
/// @param size : nombre d'espaces
/// @return chaîne de caractères contenant des espaces
std::string createSpaceString(int size);

/// @brief Retourne un entier aléatoire entre min inclue et max exclue
/// @param min borne minimale inclue
/// @param max borne maximale exclue
/// @return nombre aléatoire
int getRand(int min, int max);

/// @brief Divise un string en un vecteur de string séparé par un delimiter
/// @param str String à diviser
/// @param delimiter Délimiteur
/// @return Vecteur de string
std::vector<std::string> splitString(std::string str, const char delimiter[2]);

/// @brief Remplace une chaîne de caractères par une autre dans une chaîne de caractères
/// @param str Chaîne de caractères
/// @param toReplace Chaîne de caractères à remplacer
/// @param replaceBy Chaîne de caractères par laquelle remplacer
/// @return Chaîne de caractères modifiée
std::string replaceString(std::string str, std::string toReplace, std::string replaceBy);

/// @brief Retourne une chaîne de caractères de longueur maxLen
/// @param str Chaîne de caractères
/// @param maxLen Longueur maximale
/// @return Chaîne de caractères de longueur maxLen
std::string maxLenString(std::string str, int maxLen);

#endif