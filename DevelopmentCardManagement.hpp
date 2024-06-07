#ifndef CATAN_DEVELOPMENTCARDMANAGEMENT_HPP
#define CATAN_DEVELOPMENTCARDMANAGEMENT_HPP

#include "Catan_API.hpp"
#include "DevelopmentCard.hpp"
#include "CardDeck.hpp"
#include "Player.hpp"

namespace mycatan {
    class DevelopmentCardManagement {
    public:
        static void buyDevelopmentCard(Player* player);
        static void useMonopolyCard(Player* player, Resources giveMeThatResource);
        static void useYearOfPlentyCard(Player* player, Resources resource1, Resources resource2);
        static void getBiggestArmyCard(Player* player);
        static void tradeDevelopmentCards(Player* thisPlayer, Player* other, const std::string& cardIn, const std::string& cardOut);
        static void deleteOwnedCards(Player* player);
        static size_t getKnightCount(const Player* player);

    private:
        static Card* getOwnedCard(const Player* player, const std::string& cardType);
        static Card* getUsableCard(Player* player, const std::string& cardType);
        static void removeCard(Player* player, Card* cardToRemove);
    };
}

#endif // CATAN_DEVELOPMENTCARDMANAGEMENT_HPP
