#ifndef CATAN_CARDDECK_HPP
#define CATAN_CARDDECK_HPP

#include "DevelopmentCard.hpp"
#include "Catan_API.hpp"


namespace mycatan {
    class CardDeck {
    public:
        // Delete copy constructor and assignment operator to enforce singleton pattern
        CardDeck(const CardDeck&) = delete;
        CardDeck& operator=(const CardDeck&) = delete;

        /**
         * @brief Draw a card from the deck.
         * @return Pointer to the drawn card.
         * @throws std::runtime_error if the deck is empty.
         */
        static Card* drawCard();

        /**
         * @brief Get the current size of the deck.
         * @return The number of cards remaining in the deck.
         */
        static size_t getDeckSize();

        /**
         * @brief Destructor for CardDeck.
         * Deletes all dynamically allocated cards in the deck.
         */
        ~CardDeck();

        [[nodiscard]] static BiggestArmyCard* getBiggestArmyCard() ;

        static void cleanUp(); // Method to free resources

    private:
        static std::vector<Card*> cards;
        static size_t biggestArmyCardsInDeck;
        static bool isInitialized;

        /**
         * @brief Private constructor for the singleton pattern.
         * Initializes and shuffles the deck.
         */
        CardDeck();


        /**
         * @brief Initialize the deck with the predefined number of cards.
         */
        static void initializeDeck();

        /**
         * @brief Shuffle the deck of cards.
         */
        static void shuffleDeck();
    };

} // namespace mycatan

#endif // CATAN_CARDDECK_HPP
