#ifndef _GAME
#define _GAME

#include "Sprite.h"
#include "Player.h"
#include "Monster.h"
#include "Database.h"
#include "Synthesis.h"
#include "Fight.h"
#include "Menu.h"
#include "FightSDL.h"
#include "Entity.h"
#include "Map.h"

struct menuManager
{
    std::unordered_map<std::string, Menu> menus; // Les menus
    std::string currentMenu;                     // Le menu actuel
    std::string menuStatus;                      // Le statut du menu actuel
    std::stack<std::string> previousMenu;        // Les menus précédents
    std::stack<std::string> previousMenuStatus;  // Les statuts des menus précédents
    std::stack<int> previousChoiceX;             // Les choix précédents en X
    std::stack<int> previousChoiceY;             // Les choix précédents en Y
    int saveX;                                   // La sauvegarde du choix en X
    int saveY;                                   // La sauvegarde du choix en Y
    int stepMenu;                                // L'étape du menu
};

struct skillChanger
{
    Monster *monster;
    std::unordered_map<int, int> pointToApply;
    unsigned int pointLeft;
};

struct synthesisManager
{
    Monster *mPlus;                                       // Le monstre plus
    Monster *mMinus;                                      // Le monstre moins
    std::vector<Monster *> choicePlus;                    // Les choix de monstres plus
    std::vector<Monster *> choiceMinus;                   // Les choix de monstres moins
    std::vector<std::string> children;                    // Les enfants possibles
    bool choosingPlus;                                    // Vrai si le joueur choisi le monstre plus faux s'il choisi le monstre moins
    std::string child;                                    // L'enfant choisi
    std::unordered_map<std::string, unsigned int> skills; // Les compétences possibles
    std::vector<std::string> skillsSelected;              // Les compétences sélectionnées
};

class Game
{
private:
    // Sauvegarde et base de données
    Player player;                                       // Le joueur
    rapidjson::Document save;                            // Les données de sauvegarde
    Database database;                                   // La base de données

    // SDL
    SDL_Renderer *renderer;                              // Le renderer
    SDL_Window *window;                                  // La fenêtre
    SDL_Event event;                                     // L'évènement

    // Infos relatives au jeu
    int timeLastFrame;                                   // La mesure en tick du dernier changement de frame des sprites
    int screenWidth;                                     // La largeur de la fenêtre
    int screenHeight;                                    // La hauteur de la fenêtre
    bool running;                                        // Vrai si le jeu est en cours d'execution
    bool inMenu;                                         // Vrai si le joueur est dans un menu
    bool adminView;                                      // Vrai si l'admin view est activée    
    Camera camera;                                       // La caméra

    // Resources
    std::unordered_map<std::string, Sprite> sprites;     // Les sprites
    std::unordered_map<std::string, Mix_Chunk *> sounds; // Les sons
    std::unordered_map<std::string, Mix_Music *> musics; // Les musiques

    // Menu
    menuManager menuManager;                             // Le gestionnaire de menus
    skillChanger skillUpdater;                           // Le gestionnaire de changement de compétences
    synthesisManager synthetizer;                        // Le gestionnaire de synthèse
    Sprite cursor;                                       // Le curseur

    // Map
    Map currentMap;                                      // La carte actuelle
    std::vector<Hitbox> hitboxes;                        // Les hitboxes
    std::vector<Entity> entities;                        // Les entités
    Entity playerEntity;                                 // L'entité du joueur

public:

    // Initialisation

    /// @brief Constructeur initialise toutes les composantes de SDL
    Game();

    /// @brief Destructeur
    ~Game();

    /// @brief Lance l'execution du jeu
    void run();

    // Chargement des ressources

    /// @brief Charge les données de sauvegarde
    void loadGame();

    /// @brief Charge les sprites
    void loadSprites();

    /// @brief Charge les sons et les musiques
    void loadSounds();

    // Affichage

    /// @brief Dessine une entité
    /// @param entity L'entité à dessiner
    void drawEntity(Entity entity);

    /// @brief Dessine les contours d'une hitbox
    /// @param h La hitbox à dessiner
    void drawHitbox(Hitbox h);

    /// @brief Dessine le jeu
    void drawGame();

    /// @brief Dessine la map sauf le dernier calque
    void drawMap();

   /// @brief Dessine les infos du jeu
    void printInfos();    
    
    /// @brief Affiche les informations d'un monstre
    /// @param x Coordonnée x du monstre
    /// @param y Coordonnée y du monstre
    /// @param monster Le monstre dont on veut afficher les informations
    void drawMonsterInfo(int x, int y, Monster monster);

    /// @brief Affiche les informations partielles d'un monstre
    /// @param x Coordonnée x du monstre
    /// @param y Coordonnée y du monstre
    /// @param monster Le monstre dont on veut afficher les informations
    void drawPartMonsterInfo(int x, int y, Monster monster);

    /// @brief Affiche les informations d'une équipe
    /// @param x Coordonnée x de l'équipe
    /// @param y Coordonnée y de l'équipe
    /// @param team L'équipe dont on veut afficher les informations
    /// @param shiftX Le décalage en x entre chaque monstre
    /// @param shiftY Le décalage en y entre chaque monstre
    void drawTeamInfo(int x, int y, std::vector<Monster> team, int shiftX = 0, int shiftY = 0);

    /// @brief Affiche les informations partielles d'une équipe de monstre
    /// @param x Coordonnée x de l'équipe
    /// @param y Coordonnée y de l'équipe
    /// @param team L'équipe dont on veut afficher les informations
    /// @param shiftX Le décalage en x entre chaque monstre
    /// @param shiftY Le décalage en y entre chaque monstre
    /// @param start L'index du monstre de départ
    /// @param end L'index du monstre de fin
    void drawPartTeamInfo(int x, int y, std::vector<Monster> team, int shiftX = 0, int shiftY = 0, int start = 0, int end = 0);

    /// @brief Affiche les informations d'une équipe de pointeur sur monstre
    /// @param x Coordonnée x de l'équipe
    /// @param y Coordonnée y de l'équipe
    /// @param team L'équipe dont on veut afficher les informations
    /// @param shiftX Le décalage en x entre chaque monstre
    /// @param shiftY Le décalage en y entre chaque monstre
    /// @param start L'index du monstre de départ
    /// @param end L'index du monstre de fin
    void drawPartTeamInfo(int x, int y, std::vector<Monster *> team, int shiftX = 0, int shiftY = 0, int start = 0, int end = 0);

    /// @brief Affiche les informations d'un set de compétence
    /// @param skillSet Le set de compétence dont on veut afficher les informations
    void drawSkillSetInfo(std::string skillSet, unsigned int point = 0);

    /// @brief Affiche les menus et les informations associées à chaque menu
    void drawMenus();

    // Gestion des évenements

    /// @brief Gère les évènements tel que les touches appuyées
    void getEvents();

    /// @brief Génère une hitbox en face du joueur. Si elle touche une entité lance le script de l'entité
    void getAction();

    /// @brief Lance un script
    /// @param script Nom du script à lancer
    void runScript(std::string script);

    /// @brief Affiche un message
    /// @param message Le message à afficher
    void drawMessage(std::string message);

    /// @brief Affiche un message avec une confirmation
    /// @param message Le message à afficher
    bool askForConfirmation(std::string message);

    /// @brief Démarre un combat
    /// @param monster Monstre devant être présent dans le combat par défaut "null" donc aucun
    void startFight(std::string monster = "null");

    // Sauvegarde

    /// @brief Sauvegarde les données dans le fichier de sauvegarde
    void saveGame();

    /// @brief Supprime un monstre de la sauvegarde
    /// @param id L'id du monstre à supprimer
    void deleteFromSave(std::string id);

    /// @brief Met à jour la position de la caméra en fonction de la position du joueur
    void updateCameraPosition();

    /// @brief Met à jour les entités
    void updateEntities();

    // Gestions des menus

    /// @brief Crée les menus
    void createMenus();

    /// @brief ouvre un menu
    /// @param name Le nom du menu à ouvrir
    void openMenu(std::string name, bool canEscape = true);

    /// @brief Change le menu actuel
    /// @param name Le nom du menu à changer
    /// @param status Le status du menu à changer
    void changeMenu(std::string name, std::string status);

    /// @brief Retourne au menu précédent
    void returnToPreviousMenu();

    /// @brief Récupère le choix du joueur dans un menu
    void getPlayerChoice();

    /// @brief Definis les case du menu de choix de monstre non selectionnable
    /// @param forbidden Si vrai les cases vides sont non selectionnable Faux toute les cases sont selectionnable
    void forbiddenTeamSwap(bool forbidden = true);

    /// @brief Renvoie un pointeur sur l'équipe choisie par le joueur
    /// @param choice Le choix du joueur
    /// @return Un pointeur sur l'équipe choisie par le joueur
    std::vector<Monster> *getTeamFromChoice(int choice);

    /// @brief Met à jour le menu de stockage de monstre
    void updateStorageMonsterMenu();

    /// @brief Donne le menu d'attribution des points de compétence
    /// @param monster Le monstre dont on veut attribuer les points de compétence
    void getSkillPointsMenu(Monster monster);

    /// @brief Renvoie les vecteurs des différents choix de monstre d'une synthèse
    void getSynthesisVector();

    /// @brief Renvoie le menu de selection de monstre pour une synthèse
    /// @param charge La charge des monstres à synthétiser
    void getSynthesisMenu(std::string charge);

    /// @brief Renvoie le menu des résultats de synthèse
    void getSynthesisResultMenu();

    /// @brief Renvoie le menu de selection de compétence pour une synthèse
    void getSynthSkillMenu();
    
    /// @brief Vide les informations de synthèse
    void clearSynthesis();

    /// @brief Retourne au dernier menu précédent
    void emptyMenu();

    // Test

    /// @brief Vérifie que toutes les textures sont chargées
    void checkTextures();

    // Autres

    /// @brief Crée une équipe de monstres sauvages
    /// @param monsters Les monstres possiblement présents dans l'équipe
    /// @param levelMin Niveau minimum des monstres
    /// @param levelMax Niveau maximum des monstres
    /// @return L'équipe de monstres sauvages
    std::vector<Monster> createWildMonsterTeam(std::vector<std::string> monsters, unsigned int levelMin, unsigned int levelMax, std::string monster);

};

#endif