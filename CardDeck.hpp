#ifndef CATAN_CARDDECK_HPP
#define CATAN_CARDDECK_HPP

#include "DevelopmentCard.hpp"
#include "Catan_API.hpp"


namespace mycatan {
    class CardDeck {
    public:
        /**
         * @brief Get the singleton instance of the CardDeck.
         * @return Reference to the singleton instance of CardDeck.
         */
        static CardDeck& getInstance();

        /**
         * @brief Draw a card from the deck.
         * @return Pointer to the drawn card.
         * @throws std::runtime_error if the deck is empty.
         */
        Card* drawCard();

        /**
         * @brief Get the current size of the deck.
         * @return The number of cards remaining in the deck.
         */
        size_t getDeckSize();

        /**
         * @brief Destructor for CardDeck.
         * Deletes all dynamically allocated cards in the deck.
         */
        ~CardDeck();

        BiggestArmyCard* getBiggestArmyCard(); 

        void cleanUp(); // Method to free resources
    private:
        static std::vector<Card*> cards;
        size_t BiggestArmyCardsInDeck = 3;

        /**
         * @brief Private constructor for the singleton pattern.
         * Initializes and shuffles the deck.
         */
        CardDeck();

        // Delete copy constructor and assignment operator to enforce singleton pattern
        CardDeck(const CardDeck&) = delete;
        CardDeck& operator=(const CardDeck&) = delete;

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
