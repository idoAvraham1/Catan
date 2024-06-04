
#include "Catan.hpp"

using namespace mycatan;

Catan::Catan(Player &p1, Player &p2, Player &p3) {
     // add the players to the player vector
     this->players.push_back(&p1);
     this->players.push_back(&p2);
     this->players.push_back(&p3);

     // other set-ups
     this->currentPlayerTurn = 0;
     isWinner = false;

     // init the board
     //this->board = Board::getInstance();
}

// choose starting player randomly
size_t Catan::chooseStartingPlayer() {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, (int)players.size()-1); // define the range

    currentPlayerTurn = distr(gen); // generate random index
    std::cout << "Starting player: " << players[currentPlayerTurn]->getName() << std::endl;

    // set the player turn to true
    players[currentPlayerTurn]->setTurn(true);

    // add other players to each of the players.
    addOtherPlayersToEachPlayer();

    return currentPlayerTurn;
}
void Catan::addOtherPlayersToEachPlayer(){
    // add other players to each of the players.
    for (size_t i = 0; i < players.size(); ++i) {
        size_t nextPlayerIndex = (i + 1) % players.size();
        size_t afterNextPlayerIndex = (i + 2) % players.size();
        players[i]->addOthersPlayers(*players[nextPlayerIndex], *players[afterNextPlayerIndex]);
    }
}

std::vector<Player*> Catan::getPlayers(){
    return this->players;
}



