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

/// @brief Retourne un nombre aléatoire entre min et max
/// @param min borne minimale
/// @param max borne maximale
/// @return nombre aléatoire
int getRand(int min, int max);

#endif