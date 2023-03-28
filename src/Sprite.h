#ifndef _SPRITE
#define _SPRITE

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>
#include <vector>

struct Camera
{
    int x; // Position de la caméra en x
    int y; // Position de la caméra en y
    int w; // Largeur de l'écran
    int h; // Hauteur de l'écran
};

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

    void loadTexture(std::string path, int numSprites);

public:
    /// @brief Constructeur de la classe Sprite par copie
    /// @param other L'objet Sprite à copier
    Sprite(Sprite const &other);

    /// @brief Constructeur de la classe Sprite
    /// @param renderer Le renderer
    /// @param path Le chemin vers le fichier
    /// @param w La largeur de la texture
    /// @param h La hauteur de la texture
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
    /// @param notAbsolute Si la position est absolue ou relative à la caméra
    /// @param size La taille de la texture
    /// @param bottomCoord Si la position est au centre ou en bas de la texture
    /// @param flip Si la texture est retournée
    void draw(int x, int y, Camera c, bool notAbsolute = false, float size = 1, bool bottomCoord = false, bool flip = false);

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
};

/// @brief Dessine un rectangle noir avec un contour blanc si transparant est faux au point x,y de la taille w,h
/// @param renderer Le renderer
/// @param text Le texte à afficher
/// @param x Coordonnée en abscisse x
/// @param y Coordonnée en ordonnée y
/// @param size La taille du texte
/// @param color La couleur du texte
void drawText(SDL_Renderer *renderer, std::string text, int x, int y, int size, SDL_Color color);

/// @brief Dessine un rectangle noir avec un contour blanc
/// @param renderer Le renderer
/// @param x Coordonnée en abscisse x
/// @param y Coordonnée en ordonnée y
/// @param w Largeur
/// @param h Hauteur
void drawBox(SDL_Renderer *renderer, int x, int y, int w, int h);

/// @brief Fait changer de frame aux vecteurs de sprites
/// @param vec Le vecteur de sprites
void updateSprite(std::vector<Sprite> &vec);

#endif