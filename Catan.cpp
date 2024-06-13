// written by Ido Avraham : 208699181
// EMAIL: idoavraham086@gmail.com
#include "Catan.hpp"

using namespace mycatan;

Catan::Catan(Player& p1, Player& p2, Player& p3) : currentPlayerTurn(0), isWinner(false) {
    // Add the players to the player vector
    players.push_back(&p1);
    players.push_back(&p2);
    players.push_back(&p3);

    // Initialize the board and the dev card deck
    board = Board::getInstance();
    cardDeck = CardDeck::getInstance();
}

Catan::~Catan(){
    // Clean up
   if (board)
        board->cleanup();
   if(cardDeck)
      cardDeck->cleanup();
}

// Choose starting player randomly
size_t Catan::chooseStartingPlayer() {
    std::random_device rd;   // Obtain a random number from hardware
    std::mt19937 gen(rd());  // Seed the generator
    std::uniform_int_distribution<> distr(0, static_cast<int>(players.size()) - 1);  // Define the range

    // currentPlayerTurn = distr(gen);  // Generate random index
    currentPlayerTurn = 0;  // For simplicity, let p1 start the game ******* can be deleted *********
    std::cout << "Starting player: " << players[currentPlayerTurn]->getName() << std::endl;

    // Set the player's turn to true
    players[currentPlayerTurn]->setTurn(true);

    // Add other players to each player
    addOtherPlayersToEachPlayer();

    return currentPlayerTurn;
}

void Catan::addOtherPlayersToEachPlayer() {
    for (size_t i = 0; i < players.size(); ++i) {
        size_t nextPlayerIndex = (i + 1) % players.size();
        size_t afterNextPlayerIndex = (i + 2) % players.size();
        players[i]->addOthersPlayers(*players[nextPlayerIndex], *players[afterNextPlayerIndex]);
    }
}

std::vector<Player*> Catan::getPlayers() {
    return players;
}

bool Catan::thereIsWinner() {
    for (Player* player : players) {
        if (player->getWinningPoints() >= 10) {
            endGame(player);
            return true;
        }
    }
    // Print the winning points of each player
    std::cout << "There is no winner yet. Score state is:\n";
    for (size_t i = 0; i < players.size(); ++i) {
        std::cout << "Player " << players[i]->getName() << " has " << players[i]->getWinningPoints() << " points.\n";
    }
    return false;
}

void Catan::endGame(Player* player) {
    std::cout << player->getName() << " wins the game!" << std::endl;
}