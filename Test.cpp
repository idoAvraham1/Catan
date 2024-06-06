//written by Ido Avraham : 208699181
//EMAIL: idoavraham086@gmail.com

#include "Catan_API.hpp"
#include "doctest.h"
#include "Catan.hpp"
#include "Player.hpp"
#include "TestPlayer.hpp"
#include "DevelopmentCard.hpp"
#include "CardDeck.hpp"
#include "Board.hpp"
#include "Tile.hpp"
#include "TilesMap.hpp"

using namespace mycatan;
using namespace std;

TEST_CASE("Turns logic ") {

    // create 3 players
    Player p1("ido");
    Player p2("shoam");
    Player p3("shlomi");

    Catan catan(p1, p2, p3);

    SUBCASE("Starting player index is within valid range") {
        size_t startingPlayerIndex = catan.chooseStartingPlayer();
        CHECK(( 0 <= startingPlayerIndex && startingPlayerIndex <= 2));
    }

    SUBCASE("Player turn is correctly set") {
        size_t startingPlayerIndex = catan.chooseStartingPlayer();
        CHECK(( 0<= startingPlayerIndex && startingPlayerIndex <= 2));
        // Check if the correct player turn is set
        CHECK( (p1.getMyTurn() ==(startingPlayerIndex == 0)) ); // p1's turn is set to true if and only if startingPlayerIndex is 0.
        CHECK( (p2.getMyTurn() == (startingPlayerIndex == 1)) );
        CHECK( (p3.getMyTurn() == (startingPlayerIndex == 2)) ) ;
    }

    SUBCASE("Other players are added correctly") {
        catan.chooseStartingPlayer();
        auto players = catan.getPlayers();

        CHECK( (p1.getOtherPlayers().size() == 2) );
        CHECK(p1.getOtherPlayers()[NEXT_PLAYER]->getName() == "shoam");
        CHECK(p1.getOtherPlayers()[BEFORE_PLAYER]->getName() == "shlomi");

        CHECK( (p2.getOtherPlayers().size() == 2) );
        CHECK( (p2.getOtherPlayers()[NEXT_PLAYER]->getName() == "shlomi") );
        CHECK( (p2.getOtherPlayers()[BEFORE_PLAYER]->getName() == "ido") );

        CHECK( (p3.getOtherPlayers().size() == 2) );
        CHECK(p3.getOtherPlayers()[NEXT_PLAYER]->getName() == "ido");
        CHECK(p3.getOtherPlayers()[BEFORE_PLAYER]->getName() == "shoam");

    }

    SUBCASE("turns switches correctly") {
        size_t startingPlayerIndex = catan.chooseStartingPlayer();
        auto players = catan.getPlayers();

        // End the turn of the starting player
        players[startingPlayerIndex]->endTurn();
        CHECK(!players[startingPlayerIndex]->getMyTurn());

        // Determine the next player index
        size_t nextPlayerIndex = (startingPlayerIndex + 1) % players.size();
        CHECK(players[nextPlayerIndex]->getMyTurn());

        // End the turn of the next player
        players[nextPlayerIndex]->endTurn();
        CHECK(!players[nextPlayerIndex]->getMyTurn());

        // Determine the next player index after that
        size_t afterNextPlayerIndex = (nextPlayerIndex + 1) % players.size();
        CHECK(players[afterNextPlayerIndex]->getMyTurn());

        // End the turn of that player
        players[afterNextPlayerIndex]->endTurn();
        CHECK(!players[afterNextPlayerIndex]->getMyTurn());

        // Check that the turn returned to the starting player
        CHECK(players[startingPlayerIndex]->getMyTurn());
    }
}

TEST_CASE("Dice logic"){
    // create 3 players
    Player p1("ido");
    Player p2("shoam");
    Player p3("shlomi");

    Catan catan(p1, p2, p3);

    SUBCASE("Rolled dice result  is within valid range") {
        size_t startingPlayerIndex = catan.chooseStartingPlayer();
        auto players = catan.getPlayers();

        size_t res = players[startingPlayerIndex]->rollDice();
        CHECK(( 2 <= res && res <= 12 ));
    }

}

TEST_CASE("Test cards usage"){
    // create 3 players
    Player p1("ido");
    Player p2("shoam");
    Player p3("shlomi");
    p1.addOthersPlayers(p2,p3);
    //mycatan::Catan catan(p1, p2, p3);

    SUBCASE("Test Year of Plenty usage") {
        /**
         * This test case verifies the functionality of the Year of Plenty card.
         *
         * Steps:
         * 1. Add a Year of Plenty card to player p1.
         * 2. Set p1's turn to true.
         * 3. Use the Year of Plenty card to gain one Brick and one Wheat resource.
         * 5. Verify that p1's resource counts for Brick and Wheat are updated correctly.
         * 6. Verify that p1's turn ends after using the card.
         */

        auto* yearOfPlentyCard = new mycatan::YearOfPlentyCard();
        mycatan::TestPlayer::addCardToPlayer(p1, yearOfPlentyCard);
        p1.setTurn(true);
        p1.useYearOfPlentyCard(Resources::Brick, Resources::Wheat);
        CHECK( (TestPlayer::getResourceCount(p1, Resources::Brick) == 1) );
        CHECK((TestPlayer::getResourceCount(p1, Resources::Wheat) == 1) );
        CHECK(!p1.getMyTurn());

    }

    SUBCASE("Test Winning Points card ") {
        /**
         * This test case verifies the functionality of the Winning Points card.
         *
         * Steps:
         * 1. Add a Winning Points card to player p1.
         * 4. Verify that p1's winning points are updated correctly.
         */

        auto* winningPointsCard = new mycatan::WinningPointsCard();
        TestPlayer::addCardToPlayer(p1, winningPointsCard);
        CHECK( (p1.getWinningPoints() == 1) );
        delete winningPointsCard;
    }

    SUBCASE("Test Monopoly card usage") {
        /**
         * This test case verifies the functionality of the Monopoly card.
         *
         * Steps:
         * 1. Add a Monopoly card to player p1.
         * 2. Simulate resource addition to other players p2 and p3.
         * 3. Set p1's turn to true.
         * 4. Use the Monopoly card to collect all Brick resources from p2 and p3.
         * 5. Verify that p1 collected the correct number of Bricks from p2 and p3.
         * 7. Verify that the Brick resources of p2 and p3 are updated correctly (set to 0).
         * 8. Verify that p1's turn ends after using the card.
         */

        auto* monopolyCard = new mycatan::MonopolyCard();
        TestPlayer::addCardToPlayer(p1, monopolyCard);

        // Simulate resource addition to other players
        TestPlayer::addResources(p2,Resources::Brick, 5);
       TestPlayer::addResources(p3,Resources::Brick, 3);

        p1.setTurn(true);
        p1.useMonopolyCard(Resources::Brick);

        // Check if p1 collected the correct number of bricks from p2 and p3
        CHECK( (TestPlayer::getResourceCount(p1, Resources::Brick) == 8) );
        // Check that the bricks decreased from p2 and p3
        CHECK( (TestPlayer::getResourceCount(p2, Resources::Brick) == 0) );
        CHECK( (TestPlayer::getResourceCount(p3, Resources::Brick) == 0) );
        // Check that p1 ended his turn after using the card
        CHECK(!p1.getMyTurn());

    }

    SUBCASE("Test BiggestArmyCard"){
        /**
         * This test case verifies the functionality of the three knights card card.
         *
         * Steps:
         * 1. Add 3 knight cards to player p1.
         * 2. Set p1's turn to true.
         * 3. get the three knights card to gain 2 winning points.
         * 4. Verify that p1's winning points are updated correctly.
         * 5. Verify that that the biggest army card is marked as used
         * 6. Verify that p1's turn ends after using the card.
         */
        

         // create 3 knight cards
         auto* firstKnightCard = new KnightCard();
         auto* secondKnightCard = new KnightCard();
         auto* thirdKnightCard = new KnightCard();

         // add the cards to p1 
         TestPlayer::addCardToPlayer(p1, firstKnightCard);
         TestPlayer::addCardToPlayer(p1, secondKnightCard);
         TestPlayer::addCardToPlayer(p1, thirdKnightCard);

         //set p1 turn to true, use the card
         p1.setTurn(true);
         p1.getBiggestArmyCard();

         // verify that p1 own 4 cards, and 3 knights , gained 2 winning points
         CHECK( (p1.getOwnedCards().size() == 4) );
         CHECK( (p1.getMyTurn() == false) );
         CHECK( (p1.getWinningPoints() == 2) );

         // clean up
         TestPlayer::deletePOwnedCards(p1);
        }
}

TEST_CASE("Test card Deck ") {

    SUBCASE("Player buys development card") {
        /**
         * This test case verifies the functionality of the CardDeck and the ability of a player to buy a development card.
         *
         * Steps:
         * 1. Initialize the CardDeck.
         * 2. Draw a card and verify its type and that it is correctly removed from the deck.
         * 3. Create a player and ensure they can buy a development card if they have enough resources and it's their turn.
         * 4. Verify that the card is added to the player's owned cards and that the player's resources are decremented correctly.
         */

        // Create a player with sufficient resources
        Player p1("ido");
        p1.setTurn(true);

        // Ensure the player has enough resources
        TestPlayer::addResources(p1,Resources::Wheat, 1);
        TestPlayer::addResources(p1,Resources::Wood, 1);
        TestPlayer::addResources(p1,Resources::Wool, 1);

        // Save the original deck size
        size_t initialDeckSize = mycatan::CardDeck::getDeckSize();

        // Buy a development card
        p1.buyDevelopmentCard();
        // get the new deck size after p1 bought a card
        size_t newDeckSize =  mycatan::CardDeck::getDeckSize();

        // Ensure the deck size decreases by 1
        CHECK( (newDeckSize == initialDeckSize - 1) );

        // Verify that the player owns one more card
        CHECK( (p1.getOwnedCards().size() == 1) );

        // Verify the player's resources are decremented correctly
        CHECK( (TestPlayer::getResourceCount(p1, Resources::Wheat) == 0) );
        CHECK( (TestPlayer::getResourceCount(p1, Resources::Wood) == 0) );
        CHECK( (TestPlayer::getResourceCount(p1, Resources::Wool) == 0) );

        // Verify the card's owner is set correctly
        Card* ownedCard = p1.getOwnedCards().back();
        CHECK(ownedCard->getOwner() == p1.getName());

        // Cleanup the deck and the drawn card
        mycatan::CardDeck::cleanUp();
        delete ownedCard;
    }

    SUBCASE("Draw all the cards from the deck") {
        /**
         * This subcase verifies that all cards can be drawn from the deck and the deck becomes empty.
         *
         * Steps:
         * 1. Continuously draw cards until the deck is empty.
         * 2. Verify that the correct number of cards are drawn.
         * 3. Verify that the deck is empty after drawing all cards.
         * 4. Attempt to draw another card once the deck is empty.
         */


        size_t initialDeckSize = CardDeck::getDeckSize();
        std::vector<mycatan::Card*> drawnCards;

        // Draw all cards
        for (size_t i = 0; i < initialDeckSize; ++i) {
            mycatan::Card* card = mycatan::CardDeck::drawCard();
            REQUIRE( (card != nullptr) );
            drawnCards.push_back(card);
        }

        // Verify that the correct number of cards is drawn
        CHECK( (drawnCards.size() == initialDeckSize) );

        // Verify that the deck is empty
        size_t afterAllCardDrawedSize = CardDeck::getDeckSize();
        CHECK( (afterAllCardDrawedSize == 0) );

        // Try to draw another card after the deck is empty and catch the exception
        bool exceptionCaught = false;
        try {
           CardDeck::drawCard();
        } catch (const std::runtime_error& e) {
            exceptionCaught = true;
            CHECK(std::string(e.what()) == "Cannot draw a card. The deck is empty.");
        }
        CHECK(exceptionCaught);

        // Clean up
        for (mycatan::Card* card : drawnCards) {
            delete card;
        }
    }
}

TEST_CASE("Test trade"){
    mycatan::Player p1("ido");
    mycatan::Player p2("shoam");

    SUBCASE("Test valid resources trade"){
        //Add the players enough resources to trade
        mycatan::TestPlayer::addResources(p1,Resources::Wheat, 4);
        mycatan::TestPlayer::addResources(p2,Resources::Wood, 5);

        p1.setTurn(true);
        p1.tradeResources(&p2 , Resources::Wood , Resources::Wheat , 2 , 3 );

       // verify that the resources amount updated according to the trade preformed by the players
       CHECK( (mycatan::TestPlayer::getResourceCount(p1, Resources::Wood) == 2) );
       CHECK( (mycatan::TestPlayer::getResourceCount(p2, Resources::Wheat) == 3));

    }

    SUBCASE("Test inValid resources trade"){
        // Scenario: Player 1 doesn't have enough resources to trade
        CHECK_THROWS(p1.tradeResources(&p2 , Resources::Wood , Resources::Wheat , 6 , 3 ));
        // Scenario: Player 2 doesn't have enough resources to trade
        CHECK_THROWS(p1.tradeResources(&p2 , Resources::Wood , Resources::Wheat , 2 , 6 ));
        // Scenario: Player 1 wants to trade more resources than they have
        mycatan::TestPlayer::addResources(p1,Resources::Wheat, 2);
        CHECK_THROWS(p1.tradeResources(&p2 , Resources::Wheat , Resources::Wood , 3 , 2 ));
    }

    SUBCASE("Test valid card trade"){
        // add to the player card to trade for
        auto* yearOfPlentyCard = new mycatan::YearOfPlentyCard();
        mycatan::TestPlayer::addCardToPlayer(p1, yearOfPlentyCard);

        auto* monopolyCard = new mycatan::MonopolyCard();
        mycatan::TestPlayer::addCardToPlayer(p2, monopolyCard);

        p1.setTurn(true);
        p1.tradeDevelopmentCards(&p2, "Monopoly" , "Year of Plenty");

        // verify that both players have 1 owned card
        CHECK( (p1.getOwnedCards().size() == 1) );
        CHECK( (p2.getOwnedCards().size() == 1) );

        //verify that monopoly card owner is ido and vice versa
        CHECK(monopolyCard->getOwner() == "ido");
        CHECK(yearOfPlentyCard->getOwner() == "shoam");


        //clean up
        delete yearOfPlentyCard;
        delete monopolyCard;
    }

    SUBCASE("Test inValid card trade"){
        // Scenario: Player 1 doesn't own the specified card to trade
        CHECK_THROWS(p1.tradeDevelopmentCards(&p2, "SomeNonexistentCard" , "Year of Plenty"));
        // Scenario: Player 2 doesn't own the specified card to trade
        CHECK_THROWS(p1.tradeDevelopmentCards(&p2, "Monopoly" , "SomeNonexistentCard"));
    }

}

TEST_CASE("Test Board") {
    // Initialize the board
    mycatan::Board *board = mycatan::Board::getInstance();

    SUBCASE("Test board vertices initialization") {
        // Verify each tile on the board
        for (const auto& [coordinates, config] : Board::tileConfigurations) {
            // Find the tile inside the board
            Tile* tile = Board::getInstance()->getTile(config.numberToken, config.resourceType);

            // Retrieve the coordinates
            size_t x = coordinates.first;
            size_t y = coordinates.second;

            // Calculate the top-left vertex for the hexagon
            size_t u = 2 * (x - 1) + y;
            size_t v = y;

            // Check if the tile was found
            REQUIRE((tile != nullptr));

            // Check the tile's resource type and number token
            CHECK((tile->getResourceType() == config.resourceType));
            CHECK((tile->getId() == config.numberToken));

            // Verify the vertices of the tile
            std::vector<Vertex*> vertices = tile->getVertices();
            REQUIRE((vertices.size() == 6));

            // Check if the top-left vertex is part of the tile's vertices
            Vertex* topLeftVertex = Board::getInstance()->getVertex(u, v);
            REQUIRE((topLeftVertex != nullptr));

            bool found = false;
            for (Vertex* vertex : vertices) {
                if (vertex == topLeftVertex) {
                    found = true;
                    break;
                }
            }
            CHECK(found);

            // Further verify by checking all expected vertices coordinates
            std::vector<std::pair<size_t, size_t>> expectedVertices = {
                    {u, v}, {u + 1, v}, {u + 2, v}, {u + 2, v + 1}, {u + 1, v + 1}, {u, v + 1}
            };

            for (const auto& expected : expectedVertices) {
                Vertex* vertex = Board::getInstance()->getVertex(expected.first, expected.second);
                REQUIRE((vertex != nullptr));
                found = false;
                for (Vertex* vtx : vertices) {
                    if (vtx == vertex) {
                        found = true;
                        break;
                    }
                }
                CHECK(found);
            }
        }
    }

    SUBCASE("Test board edges initialization") {

        // Verify each tile on the board
        for (const auto& [coordinates, config] : Board::tileConfigurations) {
            // Find the tile inside the board
            Tile* tile = Board::getInstance()->getTile(config.numberToken, config.resourceType);

            // Retrieve the coordinates
            size_t x = coordinates.first;
            size_t y = coordinates.second;

            // Calculate the top-left vertex for the hexagon
            size_t u = 2 * (x - 1) + y;
            size_t v = y;

            // Verify the vertices of the tile
            std::vector<Vertex*> vertices = tile->getVertices();
            REQUIRE((vertices.size() == 6));

            // Verify the edges of the tile
            std::vector<std::pair<Vertex*, Vertex*>> expectedEdges = {
                    {Board::getInstance()->getVertex(u, v), Board::getInstance()->getVertex(u + 1, v)},
                    {Board::getInstance()->getVertex(u + 1, v), Board::getInstance()->getVertex(u + 2, v)},
                    {Board::getInstance()->getVertex(u + 2, v), Board::getInstance()->getVertex(u + 2, v + 1)},
                    {Board::getInstance()->getVertex(u + 2, v + 1), Board::getInstance()->getVertex(u + 1, v + 1)},
                    {Board::getInstance()->getVertex(u + 1, v + 1), Board::getInstance()->getVertex(u, v + 1)},
                    {Board::getInstance()->getVertex(u, v + 1), Board::getInstance()->getVertex(u, v)}
            };

            std::vector<Edge*> edges = tile->getEdges();
            REQUIRE((edges.size() == 6));

            for (const auto& expectedEdge : expectedEdges) {
                Vertex* v1 = expectedEdge.first;
                Vertex* v2 = expectedEdge.second;
                bool edgeFound = false;

                for (Edge* edge : edges) {
                    if ((edge->getVertex1() == v1 && edge->getVertex2() == v2) || (edge->getVertex1() == v2 && edge->getVertex2() == v1)) {
                        edgeFound = true;
                        break;
                    }
                }
                CHECK(edgeFound);
            }
        }
    }
}


