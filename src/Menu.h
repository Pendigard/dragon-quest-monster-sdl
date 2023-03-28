#ifndef _MENU
#define _MENU

#include "Sprite.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

enum cursorDirection
{             // Les directions du curseur par rapport à l'option
    UP = 1,   // en haut de l'option
    DOWN = 3, // en bas de l'option
    LEFT = 2, // à gauche de l'option
    RIGHT = 0 // à droite de l'option
};

enum menuChoice
{                  // Les actions à effectuer lorsqu'une option est choisie
    casterIndex,   // Choisir la cible du sort
    targetIndex,   // Choisir la cible de l'attaque
    spellIndex,    // Choisir le sort
    simulateFight, // Simuler le combat
    tacticMenu,    // Ouvrir le menu des tactiques
    scoutMenu,     // Ouvrir le menu de dressage
    orderMenu,     // Ouvrir le menu des ordres
    runAway        // Fuir le combat
};

struct Option
{                              // Une option du menu
    int x, y;                  // Position de l'option
    int w, h;                  // Taille de l'option
    std::string text;          // Texte de l'option
    bool transparent;          // Si l'option est transparente ou non
    cursorDirection direction; // La direction du curseur par rapport à l'option
    menuChoice action;         // L'action à effectuer lorsqu'une option est choisie
};

/// @brief Crée une option
/// @param text Le texte de l'option
/// @param x Coordonnée x de l'option
/// @param y Coordonnée y de l'option
/// @param w Largeur de l'option
/// @param h Hauteur de l'option
/// @param direction La direction du curseur par rapport à l'option (par défaut, à droite)
/// @param action L'action à effectuer lorsqu'une option est choisie (par défaut, retour au menu principal)
/// @param transparent Si l'option est transparente ou non (par défaut, non)
/// @return L'option créée
Option createOption(std::string text, int x, int y, int w, int h, cursorDirection direction = RIGHT, menuChoice action = targetIndex, bool transparent = false);

class Menu
{
private:
    std::vector<std::vector<std::vector<Option> > > options; // Les options du menu
    int currentChoiceX;                                    // L'index de la ligne de l'option actuelle
    int currentChoiceY;                                    // L'index de la colonne l'option actuelle
    int currentPage;                                       // L'index de la page actuelle
    Sprite *cursor;                                        // Le curseur

public:
    bool changePageX;                                      // Si on change de page avec la coordonnée X
    /// @brief Constructeur par défaut
    Menu();

    Menu(Menu const &other);
    /// @brief Constructeur de la classe Menu
    /// @param options Les options du menu
    Menu(Sprite *cursor);

    /// @brief Ajoute une ligne d'options au menu
    /// @param row La ligne d'options
    void addRow(std::vector<Option> row, int page = 0);

    /// @brief Dessine un rectangle noir avec un contour blanc si transparant est faux au point x,y de la taille w,h
    /// @param renderer Le renderer
    void drawOptions(SDL_Renderer *renderer, Camera camera);

    /// @brief Retourne l'index X de l'option actuelle
    /// @return L'index X de l'option actuelle
    int getCurrentChoiceX() const;

    /// @brief Retourne l'index Y de l'option actuelle
    /// @return L'index Y de l'option actuelle
    int getCurrentChoiceY() const;

    /// @brief Retourne l'index de la page actuelle
    /// @return L'index de la page actuelle
    int getCurrentPage() const;

    /// @brief Retourne l'action à effectuer lorsqu'une option est choisie
    /// @return L'action à effectuer lorsqu'une option est choisie
    void changeCurrentChoice(int x, int y);

    /// @brief Retourne l'action à effectuer lorsqu'une option est choisie
    /// @return L'action à effectuer lorsqu'une option est choisie
    menuChoice getChoice() const;

    /// @brief Supprime toutes les options du menu
    void clear();
};

#endif