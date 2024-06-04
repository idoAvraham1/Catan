#ifndef TEST_PLAYER_HPP
#define TEST_PLAYER_HPP

#include "Player.hpp"
#include "Catan_API.hpp"
namespace mycatan {

    class TestPlayer {
    public:
        static void addCardToPlayer(Player& player, Card* card) {
            player.ownedCards.push_back(card);
            // increase knight count if such received
         if(card->getType() == "Knight")
            player.knightCount++;
        
          if(card->getType() == "Winning Points")
            player.winning_points++;
        }

       

        static size_t getResourceCount(Player& player , Resources resources){
            size_t resourceIndex = resourceToInt(resources);
            return player.resources[resourceIndex];
        }
    };

} // namespace mycatan

#endif // TEST_PLAYER_HPP
