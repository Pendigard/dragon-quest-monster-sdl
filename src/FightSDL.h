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
    Fight fight;                                          // Le combat en mémoire
    SDL_Renderer *renderer;                               // Le renderer
    Sprite *cursor;                                       // Le sprite du curseur pour les menus
    int timeLastFrame;                                    // Le temps du dernier frame
    Camera camera;                                        // La caméra
    std::vector<std::string> teamSprites1;                // Les sprites des monstres de l'équipe 1
    std::vector<std::string> teamSprites2;                // Les sprites des monstres de l'équipe 2
    std::vector<std::string> teamSpritesIcon1;            // Les sprites des icônes des monstres de l'équipe 1
    std::string background;                               // Le sprite du fond
    std::vector<std::string> tacticIcons;                 // Les sprites des icônes des tactiques
    std::string currentMenu;                              // Le menu actuel
    std::stack<std::string> previousMenu;                 // Les menus précédents
    fightStatus status;                                   // Le status du combat
    int stepStatus;                                       // L'étape du status
    std::unordered_map<std::string, Menu> menus;          // Les menus du combat
    std::vector<Action> orders;                           // Les monstres ciblés
    int caster;                                           // Le monstre qui lance le sort -1 non défini de 0 à 2 équipe du joueur de 3 à 5 équipe adverse
    int spellChoice;                                      // Le sort choisi
    Tactics tacticIndex;                                  // La tactique choisie
    Database *database;                                   // La base de donnée
    int timeSaver;                                        // Le dernier temps où un sort a impacté
    std::queue<spellImpact> spellImpacts;                 // Les impacts de sorts
    bool pointToAlly;                                     // Si la camera pointe vers l'ennemi
    unsigned int goldReward;                              // La récompense en or
    unsigned int xpReward;                                // La récompense en xp
    std::unordered_map<std::string, Mix_Chunk *> *sounds; // Les sons
    std::unordered_map<std::string, Sprite> *sprites;     // Les sprites
    bool fleed;                                           // Si le joueur a fuit
    bool scouted;                                         // Si le joueur a dresser un monstre
    bool over;                                            // Si le combat est terminé
    bool team1Alive;                                     // Si l'équipe du joueur est vivante
    Monster *monsterScouted;                              // Le monstre dressé

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
    void drawTeamInfo(std::vector<Monster> &team, std::vector<std::string> &teamIcons);

    /// @brief Effectue différentes actions en fonction de l'input du joueur
    void getPlayerFightChoice();

    /// @brief Vérifie si tout les choix nécéssaire au statut du combat sont fait
    void checkChoiceSet();

    /// @brief Créer tout les menus du combat
    void createMenu();

    /// @brief Change le menu actuel
    /// @param id L'id du menu
    void changeCurrentMenu(std::string id);

    /// @brief Retourne au menu précédent
    void returnToPreviousMenu();

    /// @brief Ajoute une action au vecteur d'ordre
    /// @param action L'action à ajouter
    void addOrder(Action action);

    /// @brief Supprime une action du vecteur d'ordre
    /// @param idCaster L'id du lanceur
    void removeOrder(std::string idCaster);

    /// @brief Affiche les ordres
    void drawOrder();

    /// @brief Retourne l'index du monstre dans le vecteur équipe
    /// @param id L'id du monstre
    /// @param team Vrai si l'équipe est celle du joueur faux sinon
    /// @return L'index du monstre
    int getMonsterIndex(std::string id, bool &team);

    /// @brief Met à jour les menus
    void updateMonsterMenu();

    /// @brief Applique l'xp gagné aux monstres
    /// @return Les messages à afficher
    std::vector<std::string> applyXp();

    /// @brief Simule un dressage de monstre
    void simulateScout();

    /// @brief Simule la fuite de l'équipe
    void simulateFlee();

    /// @brief Simule le combat
    void simulateFight();

    /// @brief Termine le combat en affichant les récompenses
    void endFight();

    /// @brief Démarre le tour de l'équipe adverse
    void startEnemyTurn();

public:
    /// @brief Constructeur
    FightSDL(Fight &fight, SDL_Renderer *renderer, Sprite *cursor, Database *database, std::unordered_map<std::string, Sprite> *sprites, std::unordered_map<std::string, Mix_Chunk *> *sounds);

    /// @brief Destructeur
    ~FightSDL();

    /// @brief Lance le combat
    /// @return Si le combat est terminé
    bool runFight();

    /// @brief Retourne la récompense en or
    /// @return La récompense en or
    unsigned int getGoldReward() const;

    /// @brief Retourne la récompense en xp
    /// @return La récompense en xp
    unsigned int getXpReward() const;

    /// @brief Retourne l'équipe en fonction de l'index
    /// @param teamIndex L'index de l'équipe, 1 pour l'équipe du joueur, 2 pour l'équipe adverse
    /// @return L'équipe associée à l'index
    std::vector<Monster> getTeam(const unsigned int &teamIndex) const;

    /// @brief Retourne vrai si le joueur a fuit
    /// @return Vrai si le joueur a fuit
    bool getFleed() const;

    /// @brief Retourne vrai si le joueur a dresser un monstre
    /// @return Vrai si le joueur a dresser un monstre
    bool getScouted() const;

    /// @brief Le monstre dréssé si le joueur en a dressé un, un pointeur null sinon
    /// @return Le monstre dréssé 
    Monster *getMonsterScouted() const;
};

#endif