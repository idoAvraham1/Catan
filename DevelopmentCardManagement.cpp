#include "DevelopmentCardManagement.hpp"
#include "Player.hpp"

using namespace mycatan;


void DevelopmentCardManagement::buyDevelopmentCard(Player *player) {
    // Check if the player has enough resources and it is their turn
    if (!ResourceManagement::hasEnoughResources(player, "developmentCard") || !player->getMyTurn())
        throw std::runtime_error("Cannot buy development card! Check your resources and play only on your turn.");

    // Deduct the resources for buying a development card
    ResourceManagement::decreaseResourcesAfterAction(player, "developmentCard");

    // Draw a new card from the deck, set the new owner of the card
    Card *newCard = CardDeck::drawCard();
    newCard->setOwner(player);
    player->ownedCards.push_back(newCard);

    // Update the player's knight count or winning points based on the card type
    if (newCard->getType() == "Knight")
        player->knightCount++;
    else if (newCard->getType() == "Winning Points")
        player->winning_points += 1;

    std::cout << player->getName() << " drew a " << newCard->getType() << " card." << std::endl;
}


void DevelopmentCardManagement::useMonopolyCard(Player *player, Resources giveMeThatResource) {
    // Get the usable Monopoly card
    Card *monopolyCard = getUsableCard(player, "Monopoly");

    // Collect all the resources of the specified type from other players
    size_t totalResourcesCollected = 0;
    for (auto &otherPlayer: player->getOtherPlayers()) {
        totalResourcesCollected += ResourceManagement::giveAllResourcesOfType(otherPlayer, giveMeThatResource);
    }

    // Add the resources to this player
    ResourceManagement::addResource(player, giveMeThatResource, totalResourcesCollected);

    std::cout << player->name << " used a Monopoly card! Collected " << totalResourcesCollected
              << " " << resourceToString(giveMeThatResource) << " from other players." << std::endl;

    delete monopolyCard; // Delete the used card
    player->endTurn();
}


void DevelopmentCardManagement::useYearOfPlentyCard(Player *player, Resources resource1, Resources resource2) {
    // Get the usable Year of Plenty card
    Card *yearOfPlentyCard = getUsableCard(player, "Year of Plenty");

    // Add the specified resources to the player
    ResourceManagement::addResource(player, resource1, 1);
    ResourceManagement::addResource(player, resource2, 1);

    std::cout << player->name << " used a Year of Plenty card! Gained "
              << resourceToString(resource1) << " and " << resourceToString(resource2) << "." << std::endl;

    delete yearOfPlentyCard; // Delete the used card
    player->endTurn();
}


void DevelopmentCardManagement::getBiggestArmyCard(Player *player) {
    // Check if the player has enough knight cards and it is their turn
    if (player->knightCount < 3 || !player->getMyTurn())
        throw std::runtime_error("Not enough knight cards to get the biggest army card! / Play at your turn.");

    // Get a new BiggestArmyCard from the deck and add it to the player's cards
    BiggestArmyCard *biggestArmyCard = CardDeck::getBiggestArmyCard();
    player->ownedCards.push_back(biggestArmyCard);



    // Add points to this player
    player->winning_points += 2;
    std::cout << player->name << " received the biggest army card!" << std::endl;
    player->endTurn();
}


void DevelopmentCardManagement::tradeDevelopmentCards(Player *thisPlayer, Player *other, const std::string &cardIn,
                                                      const std::string &cardOut) {
    // Verify that both players own the desired cards
    Card *card_In = getOwnedCard(other, cardIn);
    Card *card_Out = getOwnedCard(thisPlayer, cardOut);
    if (card_In == nullptr || card_Out == nullptr)
        throw std::runtime_error("Players don't have the specified card, or the cards don't exist.");

    // Remove the cards from their current owners
    removeCard(thisPlayer, card_Out);
    removeCard(other, card_In);

    // handle the incoming card for each player
    handleNewDevCard(thisPlayer , card_In);
    handleNewDevCard(other , card_Out);

    std::cout << thisPlayer->name << " traded " << card_Out->getType() << " card with " << other->name << " for "
              << card_In->getType() << " card." << std::endl;
}

void DevelopmentCardManagement::handleNewDevCard(Player* player,Card* cardIn){
    // set the new owner, and add the new card to the player
    cardIn->setOwner(player);
    player->ownedCards.push_back(cardIn);

    // update player stats according to the new card
    if(cardIn->getType() == "Knight")
        player->knightCount++;
    if(cardIn->getType() == "Winning Points")
        player->winning_points++;
}

void DevelopmentCardManagement::deleteOwnedCards(Player *player) {
    for (Card *card: player->ownedCards) {
        delete card;
    }
    player->ownedCards.clear();
}


size_t DevelopmentCardManagement::getKnightCount(const Player *player) {
    return player->knightCount;
}


Card *DevelopmentCardManagement::getOwnedCard(const Player *player, const std::string &cardType) {
    for (Card *myCard: player->ownedCards) {
        if (myCard->getType() == cardType) {
            return myCard;
        }
    }
    return nullptr;
}


Card *DevelopmentCardManagement::getUsableCard(Player *player, const std::string &cardType) {
    // Get the owned card of the specified type
    Card *baseCard = getOwnedCard(player, cardType);

    // Check if the player owns the card
    if (!player->getMyTurn() || baseCard == nullptr) {
        throw std::runtime_error("You don't own this card: " + cardType);
    }
    baseCard->setUsed();
    return baseCard;
}


void DevelopmentCardManagement::removeCard(Player *player, Card *cardToRemove) {
    bool found = false;

    // Update variables according to the removed card
    for (Card *ownedCard : player->ownedCards) {
        if (ownedCard->getType() == cardToRemove->getType()) {
            found = true;
            if (ownedCard->getType() == "Knight") {
                player->knightCount--;

                // Check if the player no longer has 3 Knight cards
                if (player->knightCount < 3 && player->owningBiggestArmyCard) {
                    returnBiggestArmyCardToDeck(player);
                }
            }
            if (ownedCard->getType() == "Winning Points") {
                player->winning_points--;
            }
            break;  // Exit the loop after finding the card
        }
    }

    // Erase the card to be removed if found
    if (found) {
        auto it = std::find(player->ownedCards.begin(), player->ownedCards.end(), cardToRemove);
        if (it != player->ownedCards.end()) {
            player->ownedCards.erase(it);
        }
    }else {
        throw std::runtime_error(player->name + " does not own the card: " + cardToRemove->getType());
    }
}

void DevelopmentCardManagement::returnBiggestArmyCardToDeck(Player *player) {
    // Find and remove the Biggest Army card from the player
    Card* biggestArmyCard = nullptr;
    for (Card* card : player->ownedCards) {
        if (card->getType() == "Biggest Army") {
            biggestArmyCard = card;
            break;
        }
    }
    // the biggest Army card was found
    if (biggestArmyCard) {
        player->ownedCards.erase(std::remove(player->ownedCards.begin(), player->ownedCards.end(), biggestArmyCard), player->ownedCards.end());
        player->winning_points -= 2;
        player->owningBiggestArmyCard = false;
        CardDeck::returnBiggestArmyCard(biggestArmyCard);
    }
}
