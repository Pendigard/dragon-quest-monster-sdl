#ifndef _MAP
#define _MAP

#include "jsonFunction.h"
#include "Sprite.h"
#include "Database.h"
#include "Hitbox.h"
#include "Entity.h"

class Map
{
private:
    int width;                                      // Largeur de la map
    int height;                                     // Hauteur de la map
    std::vector<std::vector<int> > map;             // Première dimension : couche, deuxième dimension : case
    std::unordered_map<int, std::string> tileSets;  // Clé : firstGid, Valeur : TileSet
    std::vector<int> firstGids;                     // Liste des firstGids des TileSets
    std::vector<Hitbox> hitboxes;                   // Les hitboxes
    std::vector<Entity> entities;                   // Les entités
    unsigned int levelMin;                          // Le niveau minimum des monstres
    unsigned int levelMax;                          // Le niveau maximum des monstres
    unsigned int respawnX;                          // La position de respawn en x
    unsigned int respawnY;                          // La position de respawn en y
    std::vector<std::string> monsters;              // Les monstres possibles en combat

    /// @brief retourne le tileset correspondant au gid
    /// @param gid Le gid à rechercher
    /// @return Le tileset correspondant
    std::string getTileSet(int gid) const;

    /// @brief Retourne le gid d'un tileset à partir de la valeur d'une case
    /// @param value La valeur de la case
    /// @return Le gid du tileset
    int getFirstGid(int value) const;

public:
    /// @brief Constructeur
    Map();

    /// @brief Charge une map
    /// @param path Le chemin vers le fichier de la map
    void loadMap(std::string path, Database &db);

    /// @brief Retourne la largeur de la map
    /// @return La largeur de la map
    int getWidth() const;

    /// @brief Retourne la hauteur de la map
    /// @return La hauteur de la map
    int getHeight() const;

    /// @brief Retourne le nombre de calques de la map
    /// @return Le nombre de calques de la map
    int getLayerCount() const;

    /// @brief Retourne la valeur d'une case
    /// @param x Coordonnée x de la case
    /// @param y Coordonnée y de la case
    /// @param layer Couche de la case
    /// @return La valeur de la case
    int getValue(int x, int y, int layer) const;

    /// @brief Dessine un calque de la map
    /// @param layer Indice du calque à dessiner
    /// @param renderer Le renderer
    /// @param camera La caméra
    /// @param sprites Les sprites
    /// @param db La base de données
    void drawLayer(int layer, SDL_Renderer *renderer, Camera camera, std::unordered_map<std::string, Sprite> &sprites, Database &db) const;

    /// @brief Retourne les hitboxes de la map
    /// @return Les hitboxes de la map
    std::vector<Hitbox> getHitboxes() const;

    /// @brief Retourne les entités de la map
    /// @return Les entités de la map
    std::vector<Entity> getEntities() const;

    /// @brief Retourne le niveau minimum des monstres
    /// @return Le niveau minimum des monstres
    std::vector<std::string> getMonsters(unsigned int &levelMin, unsigned int &levelMax) const;

    /// @brief Retourne la position de respawn
    /// @param x Coordonnée x
    /// @param y Coordonnée y
    void getRespawnCoord(unsigned int &x, unsigned int &y) const;
};

/// @brief Retourne si une case est dans la caméra
/// @param c La caméra
/// @param x Coordonnée x de la case
/// @param y Coordonnée y de la case
/// @return Si la case est dans la caméra
bool tileInCamera(Camera &c, int x, int y);

#endif