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

class Game
{
private:
    Player player;                                     // Le joueur
    rapidjson::Document save;                          // Les données de sauvegarde
    Database database;                                 // La base de données
    Camera camera;                                     // La caméra
    SDL_Renderer *renderer;                            // Le renderer
    SDL_Window *window;                                // La fenêtre
    int timeLastFrame;                                 // La mesure en tick du dernier changement de frame des sprites
    int screenWidth;                                   // La largeur de la fenêtre
    int screenHeight;                                  // La hauteur de la fenêtre
    bool running;                                      // Vrai si le jeu est en cours d'execution
    std::unordered_map<std::string, Sprite *> sprites; // Les sprites
    std::vector<Entity> entities;                      // Les entités
    Sprite cursor;                                     // Le curseur

public:
    /// @brief Constructeur initialise toutes les composantes de SDL
    Game();

    /// @brief Destructeur
    ~Game();

    /// @brief Lance l'execution du jeu
    void run();

    /// @brief Charge les données de sauvegarde
    void loadGame();

    void loadSprites();

    void loadPlayer();

    /// @brief Sauvegarde les données dans le fichier de sauvegarde
    void saveGame();

    /// @brief Supprime un monstre de la sauvegarde
    /// @param id L'id du monstre à supprimer
    void deleteFromSave(std::string id);

    /// @brief Crée une équipe de monstres sauvages
    /// @param monsters Les monstres possiblement présents dans l'équipe
    /// @param levelMin Niveau minimum des monstres
    /// @param levelMax Niveau maximum des monstres
    /// @return L'équipe de monstres sauvages
    std::vector<Monster> createWildMonsterTeam(std::vector<std::string> monsters, unsigned int levelMin, unsigned int levelMax, std::string monster);
};

#endif