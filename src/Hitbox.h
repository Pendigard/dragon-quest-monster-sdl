#ifndef _HITBOX
#define _HITBOX

#include "Sprite.h"
#include "jsonFunction.h"

enum CollisionSide
{
    None=4,
    Top=0,
    Bottom=1,
    Left=2,
    Right=3
};

enum HitboxType
{
    Solid
};

class Hitbox
{
private:
    int x;           // Coordonnée x du coin supérieur gauche de la Hitbox
    int y;           // Coordonnée y du coin supérieur gauche de la Hitbox
    int offsetX;     // Ecart entre le coin supérieur gauche de l'entité et le coin supérieur gauche de la Hitbox
    int offsetY;     // Ecart entre le coin supérieur gauche de l'entité et le coin supérieur gauche de la Hitbox
    int w;           // Largeur de la Hitbox
    int h;           // Hauteur de la Hitbox
    HitboxType type; // Type de la Hitbox

public:
    /// @brief Constructeur par défaut de la classe Hitbox
    Hitbox();

    /// @brief Constructeur de la classe Hitbox
    /// @param x Coordonnée x du coin supérieur gauche de la Hitbox
    /// @param y Coordonnée y du coin supérieur gauche de la Hitbox
    /// @param w Largeur de la Hitbox
    /// @param h Hauteur de la Hitbox
    /// @param offsetX Ecart entre le coin supérieur gauche de l'entité et le coin supérieur gauche de la Hitbox
    /// @param offsetY Ecart entre le coin supérieur gauche de l'entité et le coin supérieur gauche de la Hitbox
    Hitbox(int x, int y, int w, int h, int offsetX = 0, int offsetY = 0,HitboxType type = Solid);

    /// @brief Constructeur par copie de la classe Hitbox avec l'opérateur =
    /// @param h Hitbox à copier
    /// @return Une copie de la Hitbox
    Hitbox &operator=(const Hitbox &h);

    /// @brief Retourne la coordonnée x du coin supérieur gauche de la Hitbox
    /// @return La coordonnée x du coin supérieur gauche de la Hitbox
    int getX() const;

    /// @brief Retourne la coordonnée y du coin supérieur gauche de la Hitbox
    /// @return La coordonnée y du coin supérieur gauche de la Hitbox
    int getY() const;

    /// @brief Retourne l'écart entre le coin supérieur gauche x de l'entité et le coin supérieur gauche de la Hitbox
    /// @return L'écart entre le coin supérieur gauche de l'entité et le coin supérieur gauche de la Hitbox
    int getOffsetX() const;

    /// @brief Retourne l'écart entre le coin supérieur gauche y de l'entité et le coin supérieur gauche de la Hitbox
    /// @return L'écart entre le coin supérieur gauche de l'entité et le coin supérieur gauche de la Hitbox
    int getOffsetY() const;

    /// @brief Renvoie le vrai x de la Hitbox
    /// @return Le vrai x de la Hitbox
    int getRealX() const;

    /// @brief Renvoie le vrai y de la Hitbox
    /// @return Le vrai y de la Hitbox
    int getRealY() const;

    /// @brief Retourne la largeur de la Hitbox
    /// @return La largeur de la Hitbox
    int getWidth() const;

    /// @brief Retourne la hauteur de la Hitbox
    /// @return La hauteur de la Hitbox
    int getHeight() const;

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

    /// @brief Retourne le côté de collision entre deux Hitbox
    /// @param h La Hitbox avec laquelle on veut tester la collision
    /// @return Le côté de collision entre deux Hitbox
    CollisionSide getCollision(Hitbox h) const;

    /// @brief Modifie x et y en fonction du côté de collision
    /// @param x La coordonnée x du coin supérieur gauche de l'entité
    /// @param y La coordonnée y du coin supérieur gauche de l'entité
    /// @param side Le côté de collision
    /// @param h La Hitbox avec laquelle on veut tester la collision
    void getHitboxBorder(int &x, int &y, CollisionSide side, Hitbox h) const;

    /// @brief Vrai si la Hitbox est en collision avec une autre Hitbox
    /// @param h La Hitbox avec laquelle on veut tester la collision
    /// @return Vrai si la Hitbox est en collision avec une autre Hitbox
    bool isColliding(Hitbox h) const;
};

#endif