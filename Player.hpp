#ifndef CATAN_PLAYER_HPP
#define CATAN_PLAYER_HPP

#include "Catan_API.hpp"
#include "DevelopmentCard.hpp"
#include "CardDeck.hpp"


namespace mycatan {

    class Player {
    public:
        // General methods
        explicit Player(std::string name);
        void endTurn();
        void addOthersPlayers(Player& otherPlayer0, Player& otherPlayer1);

        // Dice methods
        void notifyDiceRoll(size_t diceRoll);
        size_t rollDice();

        // Card methods
        void buyDevelopmentCard();
        void useMonopolyCard(Resources giveMeThatResource);
        void useYearOfPlentyCard(Resources resource1, Resources resource2);
        void use3KnightCards();
        void useWinningPointsCard();

        // Resource methods
        void addResource(Resources resource, size_t amount);
        void decreaseResource(Resources resource, size_t amount);
        size_t giveAllResourcesOfType(Resources resource);

        // Getters
        [[nodiscard]] std::string getName() const;
        [[nodiscard]] bool getMyTurn() const;
        std::vector<Player*> getOtherPlayers();
        [[nodiscard]] size_t getWinningPoints() const;
        std::vector<Card*> getOwnedCards() ;
        // Setters
        void setTurn(bool state);

    private:
        // Attributes
        std::string name;
        std::vector<size_t> resources;
        std::vector<Player*> othersPlayers;
        std::vector<Card*> ownedCards;
        bool isMyTurn;
        int winning_points;

        // Private methods
        void handleDiceRoll(size_t diceRoll);
        void decreaseResourcesAfterAction(const std::string& action);
        [[nodiscard]] bool hasEnoughResources(const std::string& action) const;
        Card* canUseCard(const std::string& cardType);

        // Helper method for card usage check
        Card* getUsableCard(const std::string& cardType);

        friend class TestPlayer; // Allow test class access to private members


    };

} // namespace mycatan

#endif // CATAN_PLAYER_HPP
