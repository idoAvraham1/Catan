// written by Ido Avraham : 208699181
// EMAIL: idoavraham086@gmail.com
#ifndef TEST_PLAYER_HPP
#define TEST_PLAYER_HPP

#include "Catan_API.hpp"
#include "DevelopmentCardManagement.hpp"
#include "Player.hpp"
#include "ResourceManagement.hpp"
namespace mycatan {

class TestPlayer {
   public:

    // Add a card to the player's owned cards
    static void addCardToPlayer(Player& player, Card* card) {
        player.ownedCards.push_back(card);
        // Increase knight count if the card is a Knight card
        if (card->getType() == "Knight")
            player.knightCount++;

        // Increase winning points if the card is a Winning Points card
        if (card->getType() == "Winning Points")
            player.winning_points++;
    }

    // Add a specified amount of a resource to the player
    static void addResources(Player& player, Resources resource, size_t amount) {
        ResourceManagement::addResource(&player, resource, amount);
    }

    // Add a specified amount of winning points to the player
    static void addWinningPoints(Player& player, size_t amount) {
        player.winning_points += amount;
    }

    // Get the count of a specified resource that the player has
    static size_t getResourceCount(Player& player, Resources resources) {
        size_t resourceIndex = resourceToInt(resources);
        return player.resources[resourceIndex];
    }

    // Delete all owned cards of the player
    static void deletePlayerOwnedCards(Player& player) {
        DevelopmentCardManagement::deleteOwnedCards(&player);
    }

    // Check if the player owns the Biggest Army card
    static bool isOwningBiggestArmyCard(Player& player) {
        return player.owningBiggestArmyCard;
    }
};

}  // namespace mycatan

#endif  // TEST_PLAYER_HPP
