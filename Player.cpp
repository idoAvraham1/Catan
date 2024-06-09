#include "Player.hpp"

using namespace mycatan;

// Constructor
Player::Player(std::string name)
        : name(std::move(name)), resources(5, 0),
        othersPlayers(2, nullptr),winning_points(0),
        isMyTurn(false), knightCount(0) , placedFirstSettlements(false)
        , placedFirstRoads(false) ,owningBiggestArmyCard(false) {}

Player::~Player() = default;

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
    // decrease half-resources to each player if 7 rolled
    if (diceRoll == 7) {
        ResourceManagement::decreaseHalfOfAllResource(this);
        for (Player* player : othersPlayers)
            if( player)
            ResourceManagement::decreaseHalfOfAllResource(player);
    }
    Board* board = Board::getInstance();
    board->allocateResources(diceRoll);
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

    // Notify the board of the dice roll
    this->notifyDiceRoll(diceRolled);

    return diceRolled;
}


// settlements and roads methods
void Player::placeSettlement(size_t x , size_t y){
    RoadAndSettlementManagement::placeSettlement(this, x, y);
}
void Player::PlaceFirstSettlements(size_t x1, size_t y1, size_t x2, size_t y2) {
    RoadAndSettlementManagement::placeFirstSettlement(this, x1, y1);
    RoadAndSettlementManagement::placeFirstSettlement(this, x2, y2);
    placedFirstSettlements = true;
}

void Player::placeFirstRoads(size_t x1, size_t y1, size_t x2, size_t y2, size_t x3, size_t y3, size_t x4, size_t y4) {
    RoadAndSettlementManagement::placeFirstRoad(this, x1, y1, x2, y2);
    RoadAndSettlementManagement::placeFirstRoad(this, x3, y3, x4, y4);
    placedFirstRoads = true ;
}

void Player::placeRoad(size_t x1, size_t y1, size_t x2, size_t y2) {
    RoadAndSettlementManagement::placeRoad(this, x1, y1, x2, y2);
}

void Player::upgradeToCity(size_t x, size_t y) {
    RoadAndSettlementManagement::upgradeToCity(this,x, y);
}

// Cards methods
void Player::buyDevelopmentCard() {
    DevelopmentCardManagement::buyDevelopmentCard(this);
}

void Player::useMonopolyCard(Resources giveMeThatResource) {
    DevelopmentCardManagement::useMonopolyCard(this, giveMeThatResource);
}

void Player::useRoadCard(size_t x1, size_t y1, size_t x2, size_t y2, size_t x3, size_t y3, size_t x4, size_t y4) {
  //  DevelopmentCardManagement::useRoadCard(this , x1, y1 , x2 , y2 , x3 , y3 , x4 , y4);
}

void Player::useYearOfPlentyCard(Resources resource1, Resources resource2) {
    DevelopmentCardManagement::useYearOfPlentyCard(this, resource1, resource2);
}

void Player::getBiggestArmyCard() {
    DevelopmentCardManagement::getBiggestArmyCard(this);
    owningBiggestArmyCard = true; // mark the player as owning biggest army card
}

void Player::tradeDevelopmentCards(Player *other, const std::string &cardIn, const std::string &cardOut) {
    DevelopmentCardManagement::tradeDevelopmentCards(this, other, cardIn, cardOut);
}


// Resource methods
void Player::tradeResources(Player *other, Resources resourceIn, Resources resourceOut, size_t inAmount, size_t outAmount) {
    ResourceManagement::tradeResources(this, other, resourceIn, resourceOut, inAmount, outAmount);
}
void Player::addResource(Resources resource, size_t amount) {
    ResourceManagement::addResource(this, resource, amount);
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

std::vector<Vertex *> Player::getSettlements() const {
    return settlements;
}

// Setters
void Player::setTurn(bool state) {
    isMyTurn = state;
}
























