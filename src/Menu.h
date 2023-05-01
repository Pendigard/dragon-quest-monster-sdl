#ifndef _MENU
#define _MENU

#include "SDL_functions.h"
#include "Sprite.h"

enum cursorDirection
{             // Les directions du curseur par rapport à l'option
    UP = 1,   // en haut de l'option
    DOWN = 3, // en bas de l'option
    LEFT = 2, // à gauche de l'option
    RIGHT = 0 // à droite de l'option
};

struct Option
{                              // Une option du menu
    int x, y;                  // Position de l'option
    int w, h;                  // Taille de l'option
    int size;                  // Taille du texte de l'option
    std::string text;          // Texte de l'option
    bool transparent;          // Si l'option est transparente ou non
    cursorDirection direction; // La direction du curseur par rapport à l'option
    bool forbidden;            // Si l'option est interdite ou non
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
Option createOption(std::string text, int x, int y, int w, int h, cursorDirection direction = RIGHT, bool transparent = false, int size = 20);

class Menu
{
private:
    int currentChoiceX;                                    // L'index de la ligne de l'option actuelle
    int currentChoiceY;                                    // L'index de la colonne l'option actuelle
    int currentPage;                                       // L'index de la page actuelle
    Sprite *cursor;                                        // Le curseur
    std::vector<std::vector<std::vector<Option> > > options; // Les options du menu

public:
    bool changePageX; // Si on change de page avec la coordonnée X

    /// @brief Constructeur par défaut
    Menu();

    /// @brief Constructeur de copie
    /// @param other Le menu à copier
    Menu(Menu const &other);

    /// @brief Constructeur de la classe Menu
    /// @param options Les options du menu
    Menu(Sprite *cursor);

    /// @brief Ajoute une option au menu
    /// @param option L'option à ajouter
    /// @param row La ligne de l'option, row <= au nombre de lignes + 1
    /// @param page La page de l'option, page <= au nombre de pages + 1
    void addOption(Option option, int row = 0, int page = 0);

    /// @brief Ajoute une ligne d'options au menu
    /// @param row La ligne d'options
    /// @param page La page de l'option, page <= au nombre de pages + 1
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

    /// @brief Retourne au premier choix du menu
    void setFirstChoice();

    /// @brief Déplace le curseur vers la droite au premier menu accessible à droite
    void changeChoiceRight();

    /// @brief Déplace le curseur vers la gauche au premier menu accessible à gauche
    void changeChoiceLeft();

    /// @brief Déplace le curseur vers le haut au premier menu accessible en haut
    void changeChoiceUp();

    /// @brief Déplace le curseur vers le bas au premier menu accessible en bas
    void changeChoiceDown();

    /// @brief Définit si le choix est interdit
    /// @param x Coordonnée x du choix
    /// @param y Coordonnée y du choix
    /// @param page La page du choix, page <= au nombre de pages
    void setForbiddenChoice(int x, int y, bool forbidden, int page = 0);

    /// @brief Retourne si le choix est interdit
    /// @return Si le choix est interdit
    bool isForbiddenChoice();

    /// @brief Supprime toutes les interdictions de choix
    void clearForbiddenChoices();

    /// @brief Supprime toutes les options du menu
    void clear();

    /// @brief Vrai si le menu est vide
    /// @return Vrai si le menu est vide
    bool empty() const;

    /// @brief Retourne le nombre de ligne du menu
    /// @return Le nombre de ligne du menu
    unsigned int getNbRow() const;
    
    /// @brief Retourne l'option choisi actuellement
    /// @return L'option choisi
    Option& getCurrentOption();
};

#endif