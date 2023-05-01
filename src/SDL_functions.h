
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "jsonFunction.h"


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

/// @brief Dessine une boîte de dialogue
/// @param renderer Le renderer
/// @param text Le texte à afficher
/// @param name Le nom du personnage
/// @param size La taille du texte
void drawDialogBox(SDL_Renderer *renderer, std::string text, std::string name, const unsigned int size = 12);

/// @brief Joue un son
/// @param sound Le son à jouer
void playSound(Mix_Chunk *sound);

/// @brief Joue une musique
/// @param music La musique à jouer
void playMusic(Mix_Music *music);