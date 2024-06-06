
#include "DevelopmentCardManagement.hpp"
#include "Player.hpp"

using namespace mycatan;

DevelopmentCardManagement::DevelopmentCardManagement(Player *player) : player(player) {}

// Buy development card
void DevelopmentCardManagement::buyDevelopmentCard() {
    if (!player->resourceManager->hasEnoughResources("developmentCard") || !player->getMyTurn())
        throw std::runtime_error("Cannot buy development card! Check your resources and play only on your turn.");

    player->resourceManager->decreaseResourcesAfterAction("developmentCard");

    Card *newCard = CardDeck::drawCard(); // draw development card from the deck
    newCard->setOwner(player);
    player->ownedCards.push_back(newCard);

    if (newCard->getType() == "Knight")
        player->knightCount++;
    else if (newCard->getType() == "Winning Points")
        player->winning_points+=1;

    std::cout << player->getName() << " drew a " << newCard->getType() << " card." << std::endl;
}

// Use Monopoly card
void DevelopmentCardManagement::useMonopolyCard(Resources giveMeThatResource) {
    // Get the usable card
    Card *monopolyCard = getUsableCard("Monopoly");


    // Collect all the resources from other players
    size_t totalResourcesCollected = 0;
    for (auto &otherPlayer: player->getOtherPlayers()) {
        totalResourcesCollected += otherPlayer->resourceManager->giveAllResourcesOfType(giveMeThatResource);
    }

    // Add the resources to this player
    player->resources[resourceToInt(giveMeThatResource)] += totalResourcesCollected;

    std::cout << player->name << " used a Monopoly card! Collected " << totalResourcesCollected
              << " " << resourceToString(giveMeThatResource) << " from other players." << std::endl;

    delete monopolyCard;
    player->endTurn();
}

// Use Year of Plenty card
void DevelopmentCardManagement::useYearOfPlentyCard(Resources resource1, Resources resource2) {
    // Get the usable card
    Card *yearOfPlentyCard = getUsableCard("Year of Plenty");

    // Add the resources to this player
    player->resources[resourceToInt(resource1)] += 1;
    player->resources[resourceToInt(resource2)] += 1;

    std::cout << player->name << " used a Year of Plenty card! Gained "
              << resourceToString(resource1) << " and " << resourceToString(resource2) << "." << std::endl;
    delete yearOfPlentyCard;
    player->endTurn();
}

// Get the Biggest Army card
void DevelopmentCardManagement::getBiggestArmyCard() {
    // verify that player own 3 knight cards
    if (player->knightCount < 3 || !player->isMyTurn)
        throw std::runtime_error("Not enough knight cards to get the biggest army Card! / play at your turn");

    // get a new ThreeKnightsCard from the deck and add him to the player cards
    BiggestArmyCard *biggestArmyCard = CardDeck::getBiggestArmyCard();
    player->ownedCards.push_back(biggestArmyCard);

    // mark card as used
    biggestArmyCard->setUsed();

    // add points to this player
    player->winning_points += 2;
    std::cout << player->name << " Received the biggest army card!" << std::endl;
    player->endTurn();
}


Card *DevelopmentCardManagement::getOwnedCard(const std::string &cardType) {
    for (Card *myCard: player->ownedCards) {
        if (myCard->getType() == cardType) {
            return myCard;
        }
    }
    return nullptr;
}

Card *DevelopmentCardManagement::getUsableCard(const std::string &cardType) {
    Card *baseCard = getOwnedCard(cardType);
    if (! player->isMyTurn || baseCard == nullptr) {
        throw std::runtime_error(" You dont own this card :" +cardType  );
    }
    baseCard->setUsed();
    return baseCard;
}

void DevelopmentCardManagement::removeCard(Card *cardToRemove) {
    bool found = false; // to mark weather, the card was found
    //update variables according to the removed card
    for (auto &ownedCard: player->ownedCards) {
        if (ownedCard->getType() == cardToRemove->getType()) {
            found = true;
            if (ownedCard->getType() == "Knight")
                player->knightCount--;
            if (ownedCard->getType() == "Winning Points")
                player->winning_points--;
        }
    }

    // erase the card to be removed if found
    if(found) {
        auto it = std::find(player->ownedCards.begin(), player->ownedCards.end(), cardToRemove);
        player->ownedCards.erase(it);
        return;
    }

    // the card wasn't found
    throw std::runtime_error(player->name +" is not owning the card:"+cardToRemove->getType());

}

void DevelopmentCardManagement::deleteOwnedCards() {
    for (Card *card: player->ownedCards) {
        delete card;
    }
}

void DevelopmentCardManagement::tradeDevelopmentCards(Player *other, const std::string &cardIn, const std::string &cardOut) {
    // verify that both of players are owning the desired cards
    Card *card_In = other->devCardManager->getOwnedCard(cardIn);
    Card *card_Out = this->player->devCardManager->getOwnedCard(cardOut);
    if(card_In == nullptr || card_Out == nullptr)
        throw std::runtime_error("Players dont have the specified card , or the cards not exists");

    card_In->setOwner(this->player);
    card_Out->setOwner(other);

    // perform the trade
    this->player->devCardManager->removeCard(card_Out);
    other->devCardManager->removeCard(card_In);
    this->player->ownedCards.push_back(card_In);
    other->ownedCards.push_back(card_Out);

    std::cout << this->player->name << " Traded " << card_Out->getType() << " card "
              << "with: " << other->name << " for " << card_In->getType() << " card" << std::endl;
}


