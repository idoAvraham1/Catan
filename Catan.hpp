
#ifndef CATAN_CATAN_HPP
#define CATAN_CATAN_HPP

#include "Catan_API.hpp"
#include "Player.hpp"
#include "Board.hpp"



namespace mycatan {
    class Catan {
    public:
        Catan(Player& p1 , Player& p2 , Player& p3);
        size_t chooseStartingPlayer();
        std::vector<Player *> getPlayers();

    private:
        //attributes
        std::vector<Player*> players;
        size_t currentPlayerTurn;
        Board* board;
        bool isWinner;
        // methods
        void addOtherPlayersToEachPlayer();

    };

}

#endif //CATAN_CATAN_HPP
