#ifndef _ENTITY
#define _ENTITY

#include "jsonFunction.h"
#include "Sprite.h"
#include "Hitbox.h"

enum Direction
{
    LOOK_UP = 0,
    LOOK_DOWN = 1,
    LOOK_LEFT = 2,
    LOOK_RIGHT = 3
};

class Entity
{
private:
    int x;                              // Coordonnée x du coin supérieur gauche de l'entité
    int y;                              // Coordonnée y du coin supérieur gauche de l'entité
    Hitbox h;                           // Hitbox de l'entité
    int speed;                          // Vitesse de l'entité
    std::vector<std::string> spritesID; // Id des Sprites de l'entité
    Direction d;                        // Direction de l'entité

public:
    /// @brief Constructeur de la classe Entity
    /// @param x Coordonnée x du coin supérieur gauche de l'entité
    /// @param y Coordonnée y du coin supérieur gauche de l'entité
    /// @param h Hitbox de l'entité
    /// @param speed Vitesse de l'entité
    /// @param s Id des Sprites de l'entité
    Entity(int x, int y, Hitbox h, int speed, std::vector<std::string> s);

    /// @brief Retourne la coordonnée x du coin supérieur gauche de l'entité
    /// @return La coordonnée x du coin supérieur gauche de l'entité
    int getX() const;

    /// @brief Retourne la coordonnée y du coin supérieur gauche de l'entité
    /// @return La coordonnée y du coin supérieur gauche de l'entité
    int getY() const;

    /// @brief Retourne la Hitbox de l'entité
    /// @return La Hitbox de l'entité
    Hitbox getHitbox() const;

    /// @brief Retourne la vitesse de l'entité
    /// @return La vitesse de l'entité
    int getSpeed() const;

    /// @brief Modifie la coordonnée x du coin supérieur gauche de l'entité
    /// @param x La nouvelle coordonnée x du coin supérieur gauche de l'entité
    /// @param y La nouvelle coordonnée y du coin supérieur gauche de l'entité
    void move(int x, int y);

    /// @brief Fait monter l'entité
    void moveUp();

    /// @brief Fait descendre l'entité
    void moveDown();

    /// @brief Fait aller l'entité à gauche
    void moveLeft();

    /// @brief Fait aller l'entité à droite
    void moveRight();
};

#endif