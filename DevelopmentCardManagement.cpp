#include "DevelopmentCardManagement.hpp"
#include "Player.hpp"

using namespace mycatan;

/**
 * @brief Buys a development card for the player.
 * @param player Pointer to the player buying the development card.
 */
void DevelopmentCardManagement::buyDevelopmentCard(Player* player) {
    if (!ResourceManagement::hasEnoughResources(player, "developmentCard") || !player->getMyTurn())
        throw std::runtime_error("Cannot buy development card! Check your resources and play only on your turn.");

    ResourceManagement::decreaseResourcesAfterAction(player, "developmentCard");

    Card* newCard = CardDeck::drawCard(); // draw development card from the deck
    newCard->setOwner(player);
    player->ownedCards.push_back(newCard);

    if (newCard->getType() == "Knight")
        player->knightCount++;
    else if (newCard->getType() == "Winning Points")
        player->winning_points += 1;

    std::cout << player->getName() << " drew a " << newCard->getType() << " card." << std::endl;
}

/**
 * @brief Uses a Monopoly card.
 * @param player Pointer to the player using the Monopoly card.
 * @param giveMeThatResource The resource type to be collected from other players.
 */
void DevelopmentCardManagement::useMonopolyCard(Player* player, Resources giveMeThatResource) {
    // Get the usable card
    Card* monopolyCard = getUsableCard(player, "Monopoly");

    // Collect all the resources from other players
    size_t totalResourcesCollected = 0;
    for (auto& otherPlayer : player->getOtherPlayers()) {
        totalResourcesCollected += ResourceManagement::giveAllResourcesOfType(otherPlayer, giveMeThatResource);
    }

    // Add the resources to this player
    ResourceManagement::addResource(player, giveMeThatResource, totalResourcesCollected);

    std::cout << player->name << " used a Monopoly card! Collected " << totalResourcesCollected
              << " " << resourceToString(giveMeThatResource) << " from other players." << std::endl;

    delete monopolyCard;
    player->endTurn();
}

/**
 * @brief Uses a Year of Plenty card.
 * @param player Pointer to the player using the Year of Plenty card.
 * @param resource1 The first resource type to be gained.
 * @param resource2 The second resource type to be gained.
 */
void DevelopmentCardManagement::useYearOfPlentyCard(Player* player, Resources resource1, Resources resource2) {
    // Get the usable card
    Card* yearOfPlentyCard = getUsableCard(player, "Year of Plenty");

    // Add the resources to this player
    ResourceManagement::addResource(player, resource1, 1);
    ResourceManagement::addResource(player, resource2, 1);

    std::cout << player->name << " used a Year of Plenty card! Gained "
              << resourceToString(resource1) << " and " << resourceToString(resource2) << "." << std::endl;

    delete yearOfPlentyCard;
    player->endTurn();
}

/**
 * @brief Awards the Biggest Army card to the player if they have enough knight cards.
 * @param player Pointer to the player receiving the Biggest Army card.
 */
void DevelopmentCardManagement::getBiggestArmyCard(Player* player) {
    // Verify that player owns 3 knight cards
    if (player->knightCount < 3 || !player->getMyTurn())
        throw std::runtime_error("Not enough knight cards to get the biggest army card! Play at your turn.");

    // Get a new BiggestArmyCard from the deck and add it to the player's cards
    BiggestArmyCard* biggestArmyCard = CardDeck::getBiggestArmyCard();
    player->ownedCards.push_back(biggestArmyCard);

    // Mark card as used
    biggestArmyCard->setUsed();

    // Add points to this player
    player->winning_points += 2;
    std::cout << player->name << " received the biggest army card!" << std::endl;
    player->endTurn();
}

/**
 * @brief Trades development cards between two players.
 * @param thisPlayer Pointer to the player initiating the trade.
 * @param other Pointer to the other player involved in the trade.
 * @param cardIn The card type the other player gives.
 * @param cardOut The card type this player gives.
 */
void DevelopmentCardManagement::tradeDevelopmentCards(Player* thisPlayer, Player* other, const std::string& cardIn, const std::string& cardOut) {
    // Verify that both players own the desired cards
    Card* card_In = getOwnedCard(other, cardIn);
    Card* card_Out = getOwnedCard(thisPlayer, cardOut);
    if (card_In == nullptr || card_Out == nullptr)
        throw std::runtime_error("Players don't have the specified card, or the cards don't exist.");

    card_In->setOwner(thisPlayer);
    card_Out->setOwner(other);

    // Perform the trade
    removeCard(thisPlayer, card_Out);
    removeCard(other, card_In);
    thisPlayer->ownedCards.push_back(card_In);
    other->ownedCards.push_back(card_Out);

    std::cout << thisPlayer->name << " traded " << card_Out->getType() << " card with " << other->name << " for " << card_In->getType() << " card." << std::endl;
}

/**
 * @brief Deletes all owned cards of the player.
 * @param player Pointer to the player whose cards will be deleted.
 */
void DevelopmentCardManagement::deleteOwnedCards(Player* player) {
    for (Card* card : player->ownedCards) {
        delete card;
    }
    player->ownedCards.clear();
}

/**
 * @brief Gets the knight count of the player.
 * @param player Pointer to the player.
 * @return The number of knight cards the player has.
 */
size_t DevelopmentCardManagement::getKnightCount(const Player* player) {
    return player->knightCount;
}

/**
 * @brief Retrieves a specific owned card of the player.
 * @param player Pointer to the player.
 * @param cardType The type of card to retrieve.
 * @return Pointer to the card if found, nullptr otherwise.
 */
Card* DevelopmentCardManagement::getOwnedCard(const Player* player, const std::string& cardType) {
    for (Card* myCard : player->ownedCards) {
        if (myCard->getType() == cardType) {
            return myCard;
        }
    }
    return nullptr;
}

/**
 * @brief Retrieves a usable card of the player.
 * @param player Pointer to the player.
 * @param cardType The type of card to retrieve.
 * @return Pointer to the usable card if found.
 * @throws std::runtime_error if the player does not own the card or it is not their turn.
 */
Card* DevelopmentCardManagement::getUsableCard(Player* player, const std::string& cardType) {
    Card* baseCard = getOwnedCard(player, cardType);
    if (!player->getMyTurn() || baseCard == nullptr) {
        throw std::runtime_error("You don't own this card: " + cardType);
    }
    baseCard->setUsed();
    return baseCard;
}

/**
 * @brief Removes a specified card from the player's owned cards.
 * @param player Pointer to the player.
 * @param cardToRemove Pointer to the card to be removed.
 * @throws std::runtime_error if the player does not own the card.
 */
void DevelopmentCardManagement::removeCard(Player* player, Card* cardToRemove) {
    bool found = false;

    // Update variables according to the removed card
    for (Card* ownedCard : player->ownedCards) {
        if (ownedCard->getType() == cardToRemove->getType()) {
            found = true;
            if (ownedCard->getType() == "Knight")
                player->knightCount--;
            if (ownedCard->getType() == "Winning Points")
                player->winning_points--;
        }
    }

    // Erase the card to be removed if found
    if (found) {
        auto it = std::find(player->ownedCards.begin(), player->ownedCards.end(), cardToRemove);
        player->ownedCards.erase(it);
        return;
    }

    // The card wasn't found
    throw std::runtime_error(player->name + " does not own the card: " + cardToRemove->getType());
}
