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

#include "../extern/rapidjson-mac/include/rapidjson/document.h"
#include "../extern/rapidjson-mac/include/rapidjson/writer.h"
#include "../extern/rapidjson-mac/include/rapidjson/stringbuffer.h"

/// @brief Convertit un json en unordered_map
/// @param json, map : json à convertir, map dans laquelle on stocke les données
void jsonToUnorderedMap(rapidjson::Value& json, std::unordered_map<std::string, unsigned int>& map);

/// @brief Convertit un json en unordered_map
/// @param json, map : json à convertir, map dans laquelle on stocke les données
void jsonToUnorderedMap(rapidjson::Value& json, std::unordered_map<std::string, float>& map);

/// @brief Charge un fichier json
/// @param path, doc : chemin du fichier, document dans lequel on stocke les données
void loadJson(std::string path, rapidjson::Document& doc);

void saveJson(std::string path, rapidjson::Document& doc);

#endif