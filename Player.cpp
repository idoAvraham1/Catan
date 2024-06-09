#include "Player.hpp"

using namespace mycatan;

// Constructor
Player::Player(std::string name)
        : name(std::move(name)), resources(5, 0),
        othersPlayers(2, nullptr),winning_points(0),
        isMyTurn(false), knightCount(0) {}

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

void Player::handleDiceRoll(size_t diceRoll) {

}

// settlements and roads methods
void Player::placeSettlement(size_t x , size_t y){
    Board* board = Board::getInstance();
    Vertex* vertex = board->getVertex(x, y);
    if (board->canPlaceSettlement(this, vertex) && ResourceManagement::hasEnoughResources(this, "settlement")) {
        vertex->buildSettlement(this);
        settlements.push_back(vertex); // Add the settlement to the player's settlements
        ResourceManagement::decreaseResourcesAfterAction(this, "settlement"); // Deduct resources for placing the settlement
        std::cout << name << " placed a settlement at (" << x << ", " << y << ")" << std::endl;
    }
    else {
        std::cout << "Cannot place settlement at (" << x << ", " << y << ")" << std::endl;
    }
}
void Player::PlaceFirstSettlements(size_t x1, size_t y1, size_t x2, size_t y2) {
    Board* board = Board::getInstance();

    // Place the first settlement without a resource and road checks
    Vertex* vertex1 = board->getVertex(x1, y1);
    if (vertex1 && !vertex1->hasSettlement()) {
        vertex1->buildSettlement(this);
        settlements.push_back(vertex1);
        std::cout << name << " placed the first settlement at (" << x1 << ", " << y1 << ")" << std::endl;
        winning_points++;
        // Allocate resources based on the first settlement location
        allocateResourcesForSettlement(vertex1);
    } else {
        std::cout << "Cannot place the first settlement at (" << x1 << ", " << y1 << ")" << std::endl;
    }

    // Place the second settlement without a resource and road checks
    Vertex* vertex2 = board->getVertex(x2, y2);
    if (vertex2 && !vertex2->hasSettlement()) {
        vertex2->buildSettlement(this);
         settlements.push_back(vertex2);
         std::cout << name << " placed the second settlement at (" << x2 << ", " << y2 << ")" << std::endl;
         winning_points ++;
        // Allocate resources based on the second settlement location
        allocateResourcesForSettlement(vertex2);
    } else {
        std::cout << "Cannot place the second settlement at (" << x2 << ", " << y2 << ")" << std::endl;
    }
}

void Player::placeFirstRoads(size_t x1, size_t y1, size_t x2, size_t y2, size_t x3, size_t y3, size_t x4, size_t y4) {
    placeRoadWithoutCheck(x1, y1, x2, y2);
    placeRoadWithoutCheck(x3, y3, x4, y4);
}

void Player::placeRoadWithoutCheck(size_t x1, size_t y1, size_t x2, size_t y2) {
    Board* board = Board::getInstance();
    Vertex* vertex1 = board->getVertex(x1, y1);
    Vertex* vertex2 = board->getVertex(x2, y2);
    Edge* edge = board->getEdge(vertex1, vertex2);

    if (vertex1 && vertex2 && edge && !edge->hasRoad()) {
        edge->placeRoad(this);
        roads.push_back(edge);
        std::cout << name << " placed a road between (" << x1 << ", " << y1 << ") and (" << x2 << ", " << y2 << ")" << std::endl;
    }
    else {
        std::cout << "Cannot place road between (" << x1 << ", " << y1 << ") and (" << x2 << ", " << y2 << ")" << std::endl;
    }
}


void Player::placeRoad(size_t x1, size_t y1, size_t x2, size_t y2) {
    Board* board = Board::getInstance();
    Vertex* vertex1 = board->getVertex(x1, y1);
    Vertex* vertex2 = board->getVertex(x2, y2);
    Edge* edge = board->getEdge(vertex1, vertex2);

    if (board->canPlaceRoad(this, vertex1, vertex2) && ResourceManagement::hasEnoughResources(this, "road")) {
        edge->placeRoad(this);
        roads.push_back(edge);
        ResourceManagement::decreaseResourcesAfterAction(this, "road"); // Deduct resources for placing the road
        std::cout << name << " placed a road between (" << x1 << ", " << y1 << ") and (" << x2 << ", " << y2 << ")" << std::endl;
    }
    else {
        std::cout << "Cannot place road between (" << x1 << ", " << y1 << ") and (" << x2 << ", " << y2 << ")" << std::endl;
    }
}



// Cards methods
void Player::buyDevelopmentCard() {
    DevelopmentCardManagement::buyDevelopmentCard(this);
}

void Player::useMonopolyCard(Resources giveMeThatResource) {
    DevelopmentCardManagement::useMonopolyCard(this, giveMeThatResource);
}

void Player::useYearOfPlentyCard(Resources resource1, Resources resource2) {
    DevelopmentCardManagement::useYearOfPlentyCard(this, resource1, resource2);
}

void Player::getBiggestArmyCard() {
    DevelopmentCardManagement::getBiggestArmyCard(this);
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
void Player::allocateResourcesForSettlement(Vertex* vertex) {
     Board* board = Board::getInstance();
    std::vector<Tile*> adjacentTiles = board->getAdjacentTiles(vertex);
    for (Tile* tile : adjacentTiles) {
        ResourceManagement::addResource(this, tile->getResourceType(), 1);
        std::cout << name << " collected 1 unit of " << resourceToString(tile->getResourceType()) << std::endl;
    }
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

std::vector<Vertex *> Player::getSettlements() const {
     return settlements;
}


















