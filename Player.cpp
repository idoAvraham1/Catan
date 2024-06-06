#include "Player.hpp"

using namespace mycatan;

// Constructor
Player::Player(std::string name)
        : name(std::move(name)), resources(5, 0),
        othersPlayers(2, nullptr),winning_points(0),
        isMyTurn(false), knightCount(0)
        ,devCardManager(new DevelopmentCardManagement(this))
        ,resourceManager(new ResourceManagement(this)) {}

Player::~Player(){
    delete devCardManager;
    delete resourceManager;

}
// General methods
void Player::endTurn() {
    isMyTurn = false;
    if (othersPlayers[NEXT_PLAYER] != nullptr) {
        othersPlayers[NEXT_PLAYER]->setTurn(true); // Start the next player's turn
    }
}

void Player::addOthersPlayers(Player &otherPlayer0, Player &otherPlayer1) {
    othersPlayers[NEXT_PLAYER] = &otherPlayer0;
    othersPlayers[BEFORE_PLAYER] = &otherPlayer1;
}

// Dice methods
void Player::notifyDiceRoll(size_t diceRoll) {
    for (Player *otherPlayer: othersPlayers) {
        if (otherPlayer != nullptr) {
            otherPlayer->handleDiceRoll(diceRoll);
        }
    }
}

size_t Player::rollDice() {
    if (!isMyTurn) {
        throw std::runtime_error("It's not your turn!");
    }

    std::random_device rd; // Obtain a random number from hardware
    std::mt19937 gen(rd()); // Seed the generator
    std::uniform_int_distribution<> distr(2, 12); // Define the range

    size_t diceRolled = distr(gen); // Generate random number
    std::cout << name << " rolled: " << diceRolled << std::endl;

    // Notify other players
    notifyDiceRoll(diceRolled);
    // Handle the dice roll for this player
    handleDiceRoll(diceRolled);

    return diceRolled;
}

void Player::handleDiceRoll(size_t diceRoll) {
    // TODO: Implement logic to handle the dice roll and update player resources
}

// cards methods
void Player::buyDevelopmentCard() {
  devCardManager->buyDevelopmentCard();
}

void Player::useMonopolyCard(Resources giveMeThatResource) {
    devCardManager->useMonopolyCard(giveMeThatResource);
}

void Player::useYearOfPlentyCard(Resources resource1, Resources resource2) {
   devCardManager->useYearOfPlentyCard(resource1,resource2);
}

void Player::getBiggestArmyCard() {
    devCardManager->getBiggestArmyCard();
}

// Resource methods
void Player::tradeResources(Player *other, Resources resourceIn, Resources resourceOut, size_t inAmount, size_t outAmount) {
    resourceManager->tradeResources(other,resourceIn,resourceOut,inAmount,outAmount);
}

// Getters
std::string Player::getName() const {
    return this->name;
}

std::vector<Edge *> Player::getRoads() const {
    return this->roads;
}

size_t Player::getWinningPoints() const {
    return this->winning_points;
}

bool Player::getMyTurn() const {
    return this->isMyTurn;
}

std::vector<Player *> Player::getOtherPlayers() {
    return this->othersPlayers;
}

std::vector<Card *> Player::getOwnedCards() {
    return ownedCards;
}

size_t Player::getKnightCount() const {
    return knightCount;
}

// Setters
void Player::setTurn(bool state) {
    isMyTurn = state;
}

// trade methods
void Player::tradeDevelopmentCards(Player *other, const std::string &cardIn, const std::string &cardOut) {
   devCardManager->tradeDevelopmentCards(other,cardIn,cardOut);
}










