#ifndef _SPRITE
#define _SPRITE

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include "jsonFunction.h"

struct Camera
{
    int x;    // Position de la caméra en x
    int y;    // Position de la caméra en y
    int w;    // Largeur de l'écran
    int h;    // Hauteur de l'écran
    int zoom; // Zoom de la caméra
};

/*
Un sprite est une texture animée ou non. Il a une largeur et une hauteur.
Il peut être composé de plusieurs textures. Chaque texture est une frame.
Il se base sur le chemin vers le fichier pour charger les textures.
Un sprite possède un pointeur vers le renderer.
On peut soit dessiner le sprite à une position donnée et le laisser gérer le
changement de frame, soit dessiner une frame donnée et gérer à l'exterieur de
la classe le changement de frame, c'est le cas pour la classe entité.
*/

class Sprite
{

private:
    SDL_Rect srcRect, destRect;         // srcRect est la zone de la texture à afficher, destRect est la zone de l'écran où on veut l'afficher
    int width, height;                  // La longueur et la largeur de la texture
    bool isAnimated;                    // Si la texture est animée
    int currentSprite;                  // L'index de la texture actuelle
    std::vector<SDL_Texture *> texture; // Les textures
    std::string path;                   // Le chemin vers le fichier
    SDL_Renderer *renderer;             // Le renderer

    /// @brief Charge la texture à partir du chemin vers le fichier
    /// @param path Le chemin vers le fichier de la forme "path/texture.png" pour une texture unique ou "path/texture_" pour une texture animée
    /// @param numSprites Le nombre de textures
    void loadTexture(std::string path, int numSprites);

public:
    /// @brief Constructeur de la classe Sprite par copie
    /// @param other L'objet Sprite à copier
    Sprite(Sprite const &other);

    /// @brief Opérateur d'affectation de la classe Sprite
    /// @param other L'objet Sprite à copier
    /// @return L'objet Sprite
    Sprite &operator=(Sprite const &other);

    /// @brief Constructeur de la classe Sprite
    /// @param renderer Le renderer
    /// @param path Le chemin vers le fichier
    /// @param numSprites Le nombre de textures
    Sprite(SDL_Renderer *renderer, std::string path, int numSprites);

    /// @brief Constructeur de la classe Sprite
    Sprite();

    /// @brief Destructeur de la classe Sprite
    ~Sprite();

    /// @brief Dessine la texture à la position x,y en fonction de la caméra
    /// @param x La coordonnée en abscisse x
    /// @param y La coordonnée en ordonnée y
    /// @param c La caméra
    /// @param size La taille de la texture
    /// @param frame L'indice de la frame à dessiner
    /// @param bottomCoord Si la position est au centre ou en bas de la texture
    /// @param flip Si la texture est retournée
    void draw(int x, int y, Camera c, float size = 1, int frame = -1, bool bottomCoord = false, bool flip = false);

    /// @brief Dessine la texture à la position x,y de manière absolue
    /// @param x La coordonnée en abscisse x
    /// @param y La coordonnée en ordonnée y
    /// @param size La taille de la texture
    /// @param frame L'indice de la frame à dessiner
    /// @param bottomCoord Si la position est au centre ou en bas de la texture
    /// @param flip Si la texture est retournée
    void draw(int x, int y, float size = 1, int frame = -1, bool bottomCoord = false, bool flip = false);

    /// @brief Dessine une partie de la texture à la position x,y en fonction de la caméra
    /// @param x La coordonnée en abscisse x
    /// @param y La coordonnée en ordonnée y
    /// @param cropX Coordonnée en abscisse x de la partie de la texture à dessiner
    /// @param cropY Coordonnée en ordonnée y de la partie de la texture à dessiner
    /// @param c La caméra
    /// @param cropW Largeur de la partie de la texture à dessiner
    /// @param cropH Hauteur de la partie de la texture à dessiner
    /// @param size La taille de la texture
    void drawCrop(int x, int y, int cropX, int cropY, Camera c, int cropW = 32, int cropH = 32, float size = 1);

    /// @brief Change la frame actuelle du sprite
    void update();

    /// @brief Change l'état d'animation du sprite
    void changeAnimation();

    /// @brief Retourne l'état d'animation du sprite
    bool getIsAnimated() const;

    /// @brief Accesseur vers la largeur de la texture
    /// @return La largeur de la texture
    int getWidth() const;

    /// @brief Accesseur vers hauteur de la texture
    /// @return La hauteur de la texture
    int getHeight() const;

    /// @brief Accesseur vers le chemin vers le fichier
    /// @return Le chemin vers le fichier
    std::string getPath() const;

    /// @brief Accesseur vers le renderer
    /// @return Le renderer
    SDL_Renderer *getRenderer() const;

    /// @brief Accesseur vers le nombre de sprites
    /// @return Le nombre de sprites
    unsigned int getNumSprites() const;

    /// @brief Change la frame actuelle du sprite
    /// @param currentSprite L'indice de la nouvelle frame
    void setCurrentSprite(int currentSprite);

    /// @brief Retourne les textures du sprite
    /// @return Les textures du sprite
    std::vector<SDL_Texture *> &getTextures();
};

/// @brief Fait changer de frame aux vecteurs de sprites
/// @param vec Le vecteur de sprites
void updateSprite(std::vector<Sprite> &vec);

/// @brief Charge le sprite d'un personnage sur la map
/// @param renderer Le renderer
/// @param name Le nom du personnage
/// @param sprites La map contenants les sprites
void loadOverworldSprite(SDL_Renderer *renderer, std::string name, std::unordered_map<std::string, Sprite> &sprites);

/// @brief Charge le sprite d'un monstre
/// @param renderer Le renderer
/// @param type Le type du monstre
/// @param sprites La map contenants les sprites
void loadMonsterSprite(SDL_Renderer *renderer, std::string type, std::unordered_map<std::string, Sprite> &sprites);

/// @brief Charge un sprite
/// @param renderer Le renderer
/// @param name Nom du sprite
/// @param path Chemin vers le fichier
/// @param sprites La map contenants les sprites
/// @param numSprites Le nombre de sprites
void loadSprite(SDL_Renderer *renderer, std::string name, std::string path, std::unordered_map<std::string, Sprite> &sprites, int numSprites = 1);

#endif