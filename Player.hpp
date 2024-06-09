#ifndef CATAN_PLAYER_HPP
#define CATAN_PLAYER_HPP

#include "Catan_API.hpp"
#include "DevelopmentCard.hpp"
#include "CardDeck.hpp"
#include "DevelopmentCardManagement.hpp"
#include "ResourceManagement.hpp"
#include "RoadAndSettlementManagement.hpp"
#include "Board.hpp"


namespace mycatan {
    class ResourceManagement;
    class DevelopmentCardManagement;
    class RoadAndSettlementManagement;
    class Edge;
    class Vertex;
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
        void useRoadCard(size_t x1 , size_t y1 , size_t x2 , size_t y2 , size_t x3 , size_t y3 , size_t x4 , size_t y4);


        // Resource methods
        void tradeResources(Player *other, Resources resourceIn , Resources resourceOut , size_t inAmount , size_t outAmount);
        void addResource(Resources resource , size_t amount);

        //settlements and road methods
        void placeSettlement(size_t x, size_t y);
        void placeRoad(size_t x1 , size_t y1 , size_t x2 , size_t y2);
        void PlaceFirstSettlements(size_t x1, size_t y1 , size_t x2, size_t y2);
        void placeFirstRoads(size_t x1 , size_t y1 , size_t x2 , size_t y2 , size_t x3 , size_t y3 , size_t x4 , size_t y4);
        void upgradeToCity(size_t x , size_t y);

        // Getters
        [[nodiscard]] std::string getName() const;
        [[nodiscard]] bool getMyTurn() const;
        std::vector<Player*> getOtherPlayers();
        [[nodiscard]] size_t getWinningPoints() const;
        std::vector<Card*> getOwnedCards() ;
        [[nodiscard]] size_t getKnightCount() const;
        [[nodiscard]]  std::vector<Edge*> getRoads() const;
        [[nodiscard]] std::vector<Vertex*> getSettlements() const;

        // Setters
        void setTurn(bool state);

    private:
        // Attributes
        std::string name;
        std::vector<size_t> resources;
        std::vector<Player*> othersPlayers;
        std::vector<Card*> ownedCards;
        std::vector<Edge*> roads;
        std::vector<Vertex*> settlements;

        bool isMyTurn;
        int winning_points;
        size_t knightCount;
        bool placedFirstSettlements;
        bool placedFirstRoads;
        bool owningBiggestArmyCard;

        // Allow Management classes to access private members
        friend class ResourceManagement;
        friend class DevelopmentCardManagement;
        friend class RoadAndSettlementManagement;
        friend class TestPlayer; // Allow test class access to private members

    };

} // namespace mycatan

#endif // CATAN_PLAYER_HPP
