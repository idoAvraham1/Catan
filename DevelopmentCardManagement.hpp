
#ifndef CATAN_DEVELOPMENTCARDMANAGEMENT_HPP
#define CATAN_DEVELOPMENTCARDMANAGEMENT_HPP

#include "Catan_API.hpp"
#include "DevelopmentCard.hpp"
#include "CardDeck.hpp"


namespace mycatan {

    class DevelopmentCardManagement {

    public:
        explicit DevelopmentCardManagement(Player *player);

        void buyDevelopmentCard();

        void useMonopolyCard(Resources giveMeThatResource);

        void useYearOfPlentyCard(Resources resource1, Resources resource2);

        void getBiggestArmyCard();

        void deleteOwnedCards();

        size_t getKnightCount() const;

        void tradeDevelopmentCards(Player *other , const std::string& cardIn , const std::string&  cardOut);

        void removeCard(Card *cardToRemove);

        // Helper methods
        Card *getOwnedCard(const std::string &cardType);

    private:
        Player* player;

        Card *getUsableCard(const std::string &cardType);

    };
}

#endif //CATAN_DEVELOPMENTCARDMANAGEMENT_HPP
