#ifndef _FIGHT_SDL
#define _FIGHT_SDL

#include "Fight.h"
#include "Sprite.h"
#include "Menu.h"

enum fightStatus
{
    mainMenu,     // Menu principal
    simulateTurn, // Si le tour est simulé
    tactic,       // Si le joueur choisit une tactique
    order,        // Si le joueur choisit un ordre
    scout,        // Si le joueur choisit un monstre à
    flee          // Si le joueur fuit
};

class FightSDL
{
private:
    Fight fight;                                 // Le combat en mémoire
    SDL_Renderer *renderer;                      // Le renderer
    Sprite *cursor;                              // Le sprite du curseur pour les menus
    int timeLastFrame;                           // Le temps du dernier frame
    Camera camera;                               // La caméra
    std::vector<Sprite> teamSprites1;            // Les sprites des monstres de l'équipe 1
    std::vector<Sprite> teamSprites2;            // Les sprites des monstres de l'équipe 2
    std::vector<Sprite> teamSpritesIcon1;        // Les sprites des icônes des monstres de l'équipe 1
    Sprite tacticIcons;                          // Les sprites des icônes des tactiques
    std::string currentMenu;                     // Le menu actuel
    std::stack<std::string> previousMenu;        // Les menus précédents
    fightStatus status;                          // Le status du combat
    int stepStatus;                              // L'étape du status
    std::unordered_map<std::string, Menu> menus; // Les menus du combat
    std::vector<Action> orders;                  // Les monstres ciblés
    int caster;                                  // Le monstre qui lance le sort -1 non défini de 0 à 2 équipe du joueur de 3 à 5 équipe adverse
    int spellChoice;                             // Le sort choisi
    Tactics tacticIndex;                         // La tactique choisie
    Database *database;                          // La base de donnée
    int lastTimeSpellImpact;                     // Le dernier temps où un sort a impacté
    std::queue<spellImpact> spellImpacts;        // Les impacts de sorts
    bool pointToAlly;                           // Si la camera pointe vers l'ennemi

    /// @brief Crée les sprites des monstres d'une équipe
    /// @param team L'équipe de monstres
    /// @return Les sprites des monstres de l'équipe
    void createTeamSprite(std::vector<Monster> &team, int teamIndex = 1);

    /// @brief Crée les sprites des icônes des monstres d'une équipe
    /// @param team L'équipe de monstres
    /// @return Les sprites des icônes des monstres de l'équipe
    void createTeamSpriteIcon(std::vector<Monster> &team);

    /// @brief Dessine une équipe de monstres
    /// @param team L'équipe de monstres
    void drawTeam(bool team1);

    /// @brief Dessine les informations d'une équipe de monstres
    /// @param team L'équipe de monstres
    /// @param teamIcons Les sprites des icônes des monstres de l'équipe
    void drawTeamInfo(std::vector<Monster> &team, std::vector<Sprite> &teamIcons);

    /// @brief Effectue différentes actions en fonction de l'input du joueur
    /// @return Si le combat doit être simuler sinon on affiche un menu
    bool getPlayerFightChoice();

    /// @brief Vérifie si tout les choix nécéssaire au statut du combat sont fait
    void checkChoiceSet();

    /// @brief Créer tout les menus du combat
    void createMenu();

    /// @brief Change le menu actuel
    void changeCurrentMenu(std::string id);

    /// @brief Retourne au menu précédent
    void returnToPreviousMenu();

    void addOrder(Action action);

    void removeOrder(std::string idCaster);

    void drawOrder();

    int getMonsterIndex(std::string id, bool &team);

    void simulateActions();

    void updateMonsterMenu();

public:
    /// @brief Constructeur
    FightSDL(Fight &fight, SDL_Renderer *renderer, Sprite *cursor, Database *database);

    /// @brief Destructeur
    ~FightSDL();

    /// @brief Lance le combat
    /// @return Si le combat est terminé
    bool runFight();
};

#endif