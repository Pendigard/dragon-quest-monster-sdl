#ifndef _ENTITY
#define _ENTITY

#include "jsonFunction.h"
#include "Sprite.h"
#include "Hitbox.h"
#include "Database.h"

enum Direction
{
    MOVE_DOWN = 0,
    MOVE_UP = 1,
    MOVE_LEFT = 2,
    MOVE_RIGHT = 3
};

enum aiType
{
    AI_NONE = 0,
    AI_RANDOM = 1,
    AI_LEFT_RIGHT = 2
};

enum entityTypes
{
    PLAYER = 0,
    NPC = 1,
    MONSTER = 2
};

struct frameManager
{
    int frame;      // Indice de la frame actuelle
    bool ascending; // Indique si l'indice de la frame doit s'incrémenter ou se décrémenter
};

/*
Une entité est un objet qui peut se déplacer sur la carte.
Elle est composée d'une hitbox, d'une position, d'une vitesse, d'une direction et d'un sprite.
Elle peut se déplacer dans les 4 directions. En générale elle est composée de 4 sprites. Un pour chaque direction.
Chacun de ces sprites est composé de 3 frames. En général la frame 2 est la frame de repos.
*/

class Entity
{
private:
    int x;                              // Coordonnée x du coin supérieur gauche de l'entité
    int y;                              // Coordonnée y du coin supérieur gauche de l'entité
    Hitbox h;                           // Hitbox de l'entité
    int speed;                          // Vitesse de l'entité
    std::vector<std::string> spritesID; // Id des Sprites de l'entité
    Direction d;                        // Direction de l'entité
    frameManager f;                     // Frame de l'entité
    bool moving;                        // Indique si l'entité se déplace ou non
    std::vector<bool> collisions;       // Indique si l'entité est en collision avec un autre objet
    aiType ai;                          // Type d'intelligence artificielle de l'entité
    entityTypes type;                   // Type de l'entité
    int timeLastMove;                   // Temps depuis le dernier déplacement de l'entité
    int timeLastFrame;                  // Temps depuis le dernier changement de frame de l'entité
    std::string name;                   // Nom de l'entité
    std::string script;                 // Script de l'entité
    int destX;                          // Coordonnée x de la destination de l'entité
    int destY;                          // Coordonnée y de la destination de l'entité

    /// @brief Change la destination de l'entité
    /// @param x Coordonnée x de la destination
    /// @param y Coordonnée y de la destination
    void changeDestination(int x, int y);

    /// @brief Défini le déplacement à effectuer pour atteindre la destination
    void moveToDestination();

    /// @brief Vrai si l'entité est à destination
    /// @return Vrai si l'entité est à destination
    bool atDestination() const;

public:
    std::vector<bool> moves; // Tableau de booléens qui indique si l'entité se déplace dans une direction

    /// @brief Constructeur par défaut de la classe Entity
    Entity();

    /// @brief Constructeur du player de la classe Entity
    /// @param x Coordonnée x du coin supérieur gauche de l'entité
    /// @param y Coordonnée y du coin supérieur gauche de l'entité
    /// @param speed Vitesse de l'entité
    /// @param s Id des Sprites de l'entité
    /// @param w Largeur de l'hitbox de l'entité
    /// @param h Hauteur de l'hitbox de l'entité
    /// @param xHitbox Ecart entre le coin supérieur gauche de l'entité et le coin supérieur gauche de l'hitbox
    /// @param yHitbox Ecart entre le coin supérieur gauche de l'entité et le coin supérieur gauche de l'hitbox
    Entity(int x, int y, int speed, std::vector<std::string> s, int w, int h, int xHitbox, int yHitbox);

    /// @brief Constructeur d'un monstre de la classe Entity
    /// @param monsterName Nom du monstre
    /// @param x Coordonnée x du coin supérieur gauche de l'entité
    /// @param y Coordonnée y du coin supérieur gauche de l'entité
    /// @param db Base de données
    /// @param ai Type d'intelligence artificielle du monstre
    /// @param d Direction ou regarde le monstre
    Entity(std::string monsterName, int x, int y, Database &db, aiType ai = AI_NONE, Direction d = MOVE_DOWN);

    /// @brief Constructeur d'un NPC de la classe Entity
    /// @param npcName Nom du NPC
    /// @param x Coordonnée x du coin supérieur gauche de l'entité
    /// @param y Coordonnée y du coin supérieur gauche de l'entité
    /// @param w Largeur de l'hitbox de l'entité
    /// @param h Hauteur de l'hitbox de l'entité
    /// @param ai Type d'intelligence artificielle du NPC
    /// @param offsetX Ecart x entre le coin supérieur gauche de l'entité et le coin supérieur gauche de l'hitbox
    /// @param offsetY Ecart y entre le coin supérieur gauche de l'entité et le coin supérieur gauche de l'hitbox
    /// @param d Direction ou regarde le NPC
    Entity(std::string npcName, int x, int y, int w, int h, aiType ai = AI_NONE, int offsetX = 0, int offsetY = 0, Direction d = MOVE_DOWN);

    /// @brief Destrcuteur de la classe Entity
    ~Entity();

    /// @brief Constructeur par copie de la classe Entity par l'opérateur =
    /// @param e L'entité à copier
    /// @return Une copie de l'entité e
    Entity &operator=(const Entity &e);

    /// @brief Constructeur par copie de la classe Entity
    /// @param e L'entité à copier
    Entity(const Entity &e);

    /// @brief Retourne la coordonnée x du coin supérieur gauche de l'entité
    /// @return La coordonnée x du coin supérieur gauche de l'entité
    int getX() const;

    /// @brief Retourne la coordonnée y du coin supérieur gauche de l'entité
    /// @return La coordonnée y du coin supérieur gauche de l'entité
    int getY() const;

    /// @brief Retourne la Hitbox de l'entité
    /// @return La Hitbox de l'entité
    Hitbox &getHitbox();

    /// @brief Retourne la vitesse de l'entité
    /// @return La vitesse de l'entité
    int getSpeed() const;

    /// @brief Change la vitesse de l'entité
    /// @param s La nouvelle vitesse de l'entité
    void setSpeed(int s);

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

    /// @brief Supprime toutes les collisions de l'entité
    void clearCollision();

    /// @brief Supprime tous les déplacements de l'entité
    void clearMoves();

    /// @brief Retourne l'id du Sprite actuel de l'entité
    /// @return L'id du Sprite actuel de l'entité
    std::string getSpriteID() const;

    /// @brief Change la frame de l'entité
    void updateFrame();

    /// @brief Met à jour l'entité
    void update();

    /// @brief Retourne la frame actuelle de l'entité
    /// @return La frame actuelle de l'entité
    int getFrame() const;

    /// @brief Définis une collision à l'entité
    /// @param side Le côté de la collision
    void setCollision(CollisionSide side);

    /// @brief Met à jour le comportement de l'entité en fonction de son IA
    void updateAi();

    /// @brief Retourne le nom de l'entité
    /// @return Le nom de l'entité
    std::string getName() const;

    /// @brief Retourne le type de l'entité
    /// @return Le type de l'entité
    entityTypes getType() const;

    /// @brief Retourne la direction qui défini où regarde l'entité
    /// @return La direction
    Direction getDirection() const;

    /// @brief Change le script de l'entité
    /// @param script Nom du nouveau script de l'entité
    void setScript(std::string script);

    /// @brief Retourne le nom du script de l'entité
    /// @return Le nom du script de l'entité
    std::string getScript() const;

    /// @brief Défini les collisions de l'entité en fonction d'une Hitbox
    /// @param h La Hitbox
    void collision(Hitbox &h);

    /// @brief Vrai si l'entité est en collision avec une autre entité
    /// @param e L'entité avec laquelle on vérifie la collision
    /// @return Vrai si l'entité est en collision avec une autre entité
    bool collided(Entity &e);

    /// @brief Vrai si l'entité est en face d'une autre entité
    /// @param e L'entité
    /// @return Vrai si l'entité est en face d'une autre entité
    bool inFrontOf(Entity &e) const;

    /// @brief Retourne l'IA de l'entité
    /// @return L'IA de l'entité
    aiType getAi() const;
};

#endif