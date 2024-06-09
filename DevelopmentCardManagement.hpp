#ifndef CATAN_DEVELOPMENTCARDMANAGEMENT_HPP
#define CATAN_DEVELOPMENTCARDMANAGEMENT_HPP

#include "Catan_API.hpp"
#include "DevelopmentCard.hpp"
#include "CardDeck.hpp"
#include "Player.hpp"

namespace mycatan {
    class DevelopmentCardManagement {
    public:
        /**
          * @brief Buys a development card for the player.
          * @param player Pointer to the player buying the development card.
          */
        static void buyDevelopmentCard(Player *player);

        /**
         * @brief Uses a Monopoly card.
         * @param player Pointer to the player using the Monopoly card.
         * @param giveMeThatResource The resource type to be collected from other players.
         */
        static void useMonopolyCard(Player *player, Resources giveMeThatResource);

        /**
         * @brief Uses a Year of Plenty card.
         * @param player Pointer to the player using the Year of Plenty card.
         * @param resource1 The first resource type to be gained.
         * @param resource2 The second resource type to be gained.
         */
        static void useYearOfPlentyCard(Player *player, Resources resource1, Resources resource2);


        static void useRoadCard(Player* player,size_t x1 , size_t y1 , size_t x2 , size_t y2 , size_t x3 , size_t y3 , size_t x4 , size_t y4);

        /**
         * @brief Awards the Biggest Army card to the player if they have enough knight cards.
         * @param player Pointer to the player receiving the Biggest Army card.
         */
        static void getBiggestArmyCard(Player *player);

        /**
          * @brief Trades development cards between two players.
          * @param thisPlayer Pointer to the player initiating the trade.
          * @param other Pointer to the other player involved in the trade.
          * @param cardIn The card type the other player gives.
          * @param cardOut The card type this player gives.
          */
        static void
        tradeDevelopmentCards(Player *thisPlayer, Player *other, const std::string &cardIn, const std::string &cardOut);

        /**
         * @brief Deletes all owned cards of the player.
         * @param player Pointer to the player whose cards will be deleted.
         */
        static void deleteOwnedCards(Player *player);

        /**
         * @brief Gets the knight count of the player.
         * @param player Pointer to the player.
         * @return The number of knight cards the player has.
         */
        static size_t getKnightCount(const Player *player);

    private:
        /**
        * @brief Retrieves a specific owned card of the player.
        * @param player Pointer to the player.
        * @param cardType The type of card to retrieve.
        * @return Pointer to the card if found, nullptr otherwise.
        */
        static Card *getOwnedCard(const Player *player, const std::string &cardType);

        /**
         * @brief Retrieves a usable card of the player.
         * @param player Pointer to the player.
         * @param cardType The type of card to retrieve.
         * @return Pointer to the usable card if found.
         * @throws std::runtime_error if the player does not own the card or it is not their turn.
         */
        static Card *getUsableCard(Player *player, const std::string &cardType);

        /**
         * @brief Removes a specified card from the player's owned cards.
         * @param player Pointer to the player.
         * @param cardToRemove Pointer to the card to be removed.
         * @throws std::runtime_error if the player does not own the card.
         */
        static void removeCard(Player *player, Card *cardToRemove);


        static void returnBiggestArmyCardToDeck(Player* player);

        static void handleNewDevCard(Player *player, Card *cardIn);
    };
}

#endif // CATAN_DEVELOPMENTCARDMANAGEMENT_HPP
