// written by Ido Avraham : 208699181
// EMAIL: idoavraham086@gmail.com
#ifndef CATAN_CARDDECK_HPP
#define CATAN_CARDDECK_HPP

#include "DevelopmentCard.hpp"
#include "Catan_API.hpp"


namespace mycatan {
    class CardDeck {
    public:
        // Delete copy constructor and assignment operator to enforce singleton pattern
        CardDeck(const CardDeck &) = delete;

        CardDeck &operator=(const CardDeck &) = delete;

        static CardDeck *getInstance();

        /**
         * @brief Draw a card from the deck.
         * @return Pointer to the drawn card.
         * @throws std::runtime_error if the deck is empty.
         */
        static Card *drawCard();

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

        /**
         * @brief Get the Biggest Army card from the deck.
         * @return Pointer to the Biggest Army card.
         * @throws std::runtime_error if no Biggest Army cards are left in the deck.
         */
        [[nodiscard]] static BiggestArmyCard *getBiggestArmyCard();


        /**
         * @brief cleans up the deck (i.e deleting all cards in the deck)
         */
        static void cleanDeck();

        /**
         * @brief Returns the Biggest Army card to the deck.
         * @param card Pointer to the Biggest Army card.
         */
        static void returnBiggestArmyCard(Card *biggestArmyCard);

    private:

        // attributes:
        static std::vector<Card *> cards; // Vector to store the cards in the deck
        static size_t biggestArmyCardsInDeck;  // Number of Biggest Army cards remaining in the deck

        // methods:
        /**
         * @brief Private constructor for the singleton pattern.
         * Initializes and shuffles the deck.
         */
        CardDeck();

        /**
        * @brief Clean up the deck by deleting all dynamically allocated cards.
        */
        static void cleanUp(); // Method to free resources


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
