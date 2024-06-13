// written by Ido Avraham : 208699181
// EMAIL: idoavraham086@gmail.com

#ifndef CATAN_CATAN_HPP
#define CATAN_CATAN_HPP

#include "Board.hpp"
#include "Catan_API.hpp"
#include "Player.hpp"

namespace mycatan {
class Catan {
public:
    // Constructor: Initializes the game with three players
    Catan(Player& p1, Player& p2, Player& p3);

    // Choose the starting player randomly and return their index
    size_t chooseStartingPlayer();

    // Get the list of players in the game
    std::vector<Player*> getPlayers();

    // Check if there is a winner in the game
    bool thereIsWinner();



private:
    // Attributes
    std::vector<Player*> players;  // List of players in the game
    size_t currentPlayerTurn;      // Index of the current player's turn
    Board* board;                  // Pointer to the game board
    CardDeck* cardDeck;            // Pointer to the card deck
    bool isWinner;                 // Flag indicating if there is a winner

    // methods

    // Add references to other players for each player
    void addOtherPlayersToEachPlayer();
    // End the game and declare the winner
    void endGame(Player* player);
};

}  // namespace mycatan

#endif  // CATAN_CATAN_HPP
