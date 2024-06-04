#include "Player.hpp"
using namespace mycatan;

// Constructor
    Player::Player(std::string name)
            : name(std::move(name)), resources(5, 0), othersPlayers(2, nullptr),
             winning_points(0), isMyTurn(false) {
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

// Card methods
    void Player::buyDevelopmentCard() {
        // verify that the player can buy a card
        if (! hasEnoughResources("developmentCard")  || !isMyTurn)
            throw std::runtime_error("Cant buy development card! check your resources and play only in your turn!");

        decreaseResourcesAfterAction("developmentCard");
        Card* newCard = CardDeck::drawCard();
        // add the drawn card to the player owned cards
        ownedCards.push_back(newCard);
        // set the new owner of the card
        newCard->setOwner(this);
    }

    void Player::useMonopolyCard(Resources giveMeThatResource) {
        // Get the usable card
        getUsableCard("Monopoly");

        // Collect all the resources from other players
        size_t totalResourcesCollected = 0;
        for (auto& otherPlayer : getOtherPlayers()) {
            totalResourcesCollected += otherPlayer->giveAllResourcesOfType(giveMeThatResource);
        }

        // Add the resources to this player
        resources[resourceToInt(giveMeThatResource)] += totalResourcesCollected;

        std::cout << name << " used a Monopoly card! Collected " << totalResourcesCollected
                  << " " << resourceToString(giveMeThatResource) << " from other players." << std::endl;
        endTurn();
    }

    void Player::useYearOfPlentyCard(Resources resource1, Resources resource2) {
        // Get the usable card
        getUsableCard("Year of Plenty");

        // Add the resources to this player
        resources[resourceToInt(resource1)] += 1;
        resources[resourceToInt(resource2)] += 1;

        std::cout << name << " used a Year of Plenty card! Gained "
                  << resourceToString(resource1) << " and " << resourceToString(resource2) << "." << std::endl;
        endTurn();
    }

    void Player::useWinningPointsCard() {
        // Get the usable card
        getUsableCard("Winning Points");

        // add points to this player
        winning_points+=2;
        std::cout << name << " used a Winning points  card!" << std::endl;
        endTurn();

    }

    Card *Player::canUseCard(const std::string &cardType) {
        for (Card *myCard: ownedCards) {
            if (myCard->getType() == cardType && !myCard->isUsed()) {
                return myCard;
            }
        }
        return nullptr;
    }

    Card* Player::getUsableCard(const std::string& cardType) {
        Card* baseCard = canUseCard(cardType);
        if (!isMyTurn || baseCard == nullptr) {
            throw std::runtime_error(cardType + " card can't be used!");
        }
        baseCard->setUsed();
        return baseCard;
    }

// Resource methods
    void Player::addResource(Resources resource, size_t amount) {
        size_t resourceIndex = resourceToInt(resource);
        resources[resourceIndex] += amount;
    }

    void Player::decreaseResource(Resources resource, size_t amount) {
        size_t resourceIndex = resourceToInt(resource);
        if (resources[resourceIndex] < amount) {
            throw std::runtime_error("Not enough resources!");
        }
        resources[resourceIndex] -= amount;
    }

    size_t Player::giveAllResourcesOfType(Resources resource) {
        size_t amount = resources[resourceToInt(resource)];
        resources[resourceToInt(resource)] = 0;
        return amount;
    }


    void Player::decreaseResourcesAfterAction(const std::string &action) {
        static std::unordered_map<std::string, std::vector<size_t>> resourceRequirements = {
                {"settlement",      {1, 1, 1, 1}},
                {"road",            {1, 1, 0, 0}},
                {"city",            {0, 2, 3, 0}},
                {"developmentCard", {0, 1, 1, 1}}
        };

        const auto &requirements = resourceRequirements[action];
        resources[WOOD] -= requirements[WOOD];
        resources[BRICK] -= requirements[BRICK];
        resources[WHEAT] -= requirements[WHEAT];
        resources[WOOL] -= requirements[WOOL];
    }

    bool Player::hasEnoughResources(const std::string &action) const {
        static std::unordered_map<std::string, std::vector<size_t>> resourceRequirements = {
                {"settlement",      {1, 1, 1, 1}},
                {"road",            {1, 1, 0, 0}},
                {"city",            {0, 2, 3, 0}},
                {"developmentCard", {0, 1, 1, 1}}
        };

        const auto &requirements = resourceRequirements[action];
        return resources[WOOD] >= requirements[WOOD] &&
               resources[BRICK] >= requirements[BRICK] &&
               resources[WHEAT] >= requirements[WHEAT] &&
               resources[WOOL] >= requirements[WOOL];
    }


// Getters
    std::string Player::getName() const {
        return this->name;
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

// Setters
    void Player::setTurn(bool state) {
        isMyTurn = state;
    }




