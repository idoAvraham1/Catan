#ifndef CATAN_PLAYER_HPP
#define CATAN_PLAYER_HPP

#include "Catan_API.hpp"
#include "DevelopmentCard.hpp"
#include "CardDeck.hpp"
#include "DevelopmentCardManagement.hpp"
#include "ResourceManagement.hpp"


namespace mycatan {
    class ResourceManagement;
    class Player {
    public:
        // General methods
        explicit Player(std::string name);
        ~Player();
        void endTurn();
        void addOthersPlayers(Player& otherPlayer0, Player& otherPlayer1);

        // Dice methods
        void notifyDiceRoll(size_t diceRoll);
        size_t rollDice();

        // Card methods
        void buyDevelopmentCard();
        void useMonopolyCard(Resources giveMeThatResource);
        void useYearOfPlentyCard(Resources resource1, Resources resource2);
        void getBiggestArmyCard();
        void tradeDevelopmentCards(Player *other , const std::string& cardIn , const std::string&  cardOut);

        // Resource methods
        void tradeResources(Player *other, Resources resourceIn , Resources resourceOut , size_t inAmount , size_t outAmount);

        // Getters
        [[nodiscard]] std::string getName() const;
        [[nodiscard]] bool getMyTurn() const;
        std::vector<Player*> getOtherPlayers();
        [[nodiscard]] size_t getWinningPoints() const;
        std::vector<Card*> getOwnedCards() ;
        [[nodiscard]] size_t getKnightCount() const;

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
        size_t knightCount;

        // Private methods
        void handleDiceRoll(size_t diceRoll);


        DevelopmentCardManagement *devCardManager; // Pointer to DevelopmentCardManagement object
        ResourceManagement *resourceManager; // Pointer to a Resource manager object


        friend class DevelopmentCardManagement; // Allow DevelopmentCardManagement to access private members
        friend class ResourceManagement; // Allow ResourceManagement to access private members
        friend class TestPlayer; // Allow test class access to private members

    };

} // namespace mycatan

#endif // CATAN_PLAYER_HPP
