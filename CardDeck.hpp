#ifndef CATAN_CARDDECK_HPP
#define CATAN_CARDDECK_HPP

#include "DevelopmentCard.hpp"
#include "Catan_API.hpp"
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>

namespace mycatan {
    class CardDeck {
    public:
        static CardDeck& getInstance();
        static Card* drawCard();
        static size_t getDeckSize();
         ~CardDeck();

    private:
        static std::vector<Card*> cards;

        CardDeck(); // Private constructor
        CardDeck(const CardDeck&) = delete; // Delete copy constructor
        CardDeck& operator=(const CardDeck&) = delete; // Delete assignment operator

        static void initializeDeck();
        static void shuffleDeck();
    };

} // namespace mycatan

#endif // CATAN_CARDDECK_HPP
