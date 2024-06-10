
#ifndef CATAN_CATAN_HPP
#define CATAN_CATAN_HPP

#include "Board.hpp"
#include "Catan_API.hpp"
#include "Player.hpp"

namespace mycatan {
class Catan {
   public:
    Catan(Player& p1, Player& p2, Player& p3);
    size_t chooseStartingPlayer();
    std::vector<Player*> getPlayers();

    bool thereIsWinner();

    void endGame(Player* player);

   private:
    // attributes
    std::vector<Player*> players;
    size_t currentPlayerTurn;
    Board* board;
    CardDeck* cardDeck;
    bool isWinner;
    // methods
    void addOtherPlayersToEachPlayer();
};

}  // namespace mycatan

#endif  // CATAN_CATAN_HPP
