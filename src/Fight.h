#include "GameTxt.h"

class Fight
{
    public:
        unsigned int nbTurn = 0;
        std::vector<Monster> team1;
        std::vector<Monster> team2;

    Fight(std::vector<Monster> t1, std::vector<Monster> t2);

}

