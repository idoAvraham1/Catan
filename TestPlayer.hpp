#ifndef TEST_PLAYER_HPP
#define TEST_PLAYER_HPP

#include "Catan_API.hpp"
#include "DevelopmentCardManagement.hpp"
#include "Player.hpp"
#include "ResourceManagement.hpp"
namespace mycatan {

class TestPlayer {
   public:
    static void addCardToPlayer(Player& player, Card* card) {
        player.ownedCards.push_back(card);
        // increase knight count if such received
        if (card->getType() == "Knight")
            player.knightCount++;

        if (card->getType() == "Winning Points")
            player.winning_points++;
    }

    static void addResources(Player& player, Resources resource, size_t amount) {
        ResourceManagement::addResource(&player, resource, amount);
    }

    static void addWinningPoints(Player& player, size_t amount) {
        player.winning_points += amount;
    }

    static size_t getResourceCount(Player& player, Resources resources) {
        size_t resourceIndex = resourceToInt(resources);
        return player.resources[resourceIndex];
    }
    static void deletePlayerOwnedCards(Player& player) {
        DevelopmentCardManagement::deleteOwnedCards(&player);
    }
    static bool isOwningBiggestArmyCard(Player& player) {
        return player.owningBiggestArmyCard;
    }
};

}  // namespace mycatan

#endif  // TEST_PLAYER_HPP
