#ifndef _HITBOX
#define _HITBOX

#include "Sprite.h"
#include "jsonFunction.h"


class Hitbox
{
private:
    int x; // Coordonnée x du coin supérieur gauche de la Hitbox
    int y; // Coordonnée y du coin supérieur gauche de la Hitbox
    int w; // Largeur de la Hitbox
    int h; // Hauteur de la Hitbox

public:

    /// @brief Constructeur par défaut de la classe Hitbox
    Hitbox();
    /// @brief Constructeur de la classe Hitbox
    /// @param x Coordonnée x du coin supérieur gauche de la Hitbox
    /// @param y Coordonnée y du coin supérieur gauche de la Hitbox
    /// @param w Largeur de la Hitbox
    /// @param h Hauteur de la Hitbox
    Hitbox(int x, int y, int w, int h);

    /// @brief Retourne la coordonnée x du coin supérieur gauche de la Hitbox
    /// @return La coordonnée x du coin supérieur gauche de la Hitbox
    int getX() const;

    /// @brief Retourne la coordonnée y du coin supérieur gauche de la Hitbox
    /// @return La coordonnée y du coin supérieur gauche de la Hitbox
    int getY() const;

    /// @brief Retourne la largeur de la Hitbox
    /// @return La largeur de la Hitbox
    int getW() const;

    /// @brief Retourne la hauteur de la Hitbox
    /// @return La hauteur de la Hitbox
    int getH() const;

    /// @brief Modifie la coordonnée x du coin supérieur gauche de la Hitbox
    /// @param x La nouvelle coordonnée x du coin supérieur gauche de la Hitbox
    void setX(int x);

    /// @brief Modifie la coordonnée y du coin supérieur gauche de la Hitbox
    /// @param y La nouvelle coordonnée y du coin supérieur gauche de la Hitbox
    void setY(int y);

    /// @brief Modifie la largeur de la Hitbox
    /// @param w La nouvelle largeur de la Hitbox
    void setW(int w);

    /// @brief Modifie la hauteur de la Hitbox
    /// @param h La nouvelle hauteur de la Hitbox
    void setH(int h);

    /// @brief Modifie la coordonnée x du coin supérieur gauche de la Hitbox
    /// @param x La nouvelle coordonnée x du coin supérieur gauche de la Hitbox
    bool isCollide(Hitbox h);
};

#endif