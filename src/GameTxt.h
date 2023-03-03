#ifndef _GAME
#define _GAME


#include "Player.h"



class Game
{
    public:
        Player player;
        rapidjson::Document monsterBase;
        rapidjson::Document save;
        rapidjson::Document skillBase;
        rapidjson::Document spellBase;
        
        /// @brief  Charge les données de save.json
        /// @return void
        void loadPlayer();

};

#endif