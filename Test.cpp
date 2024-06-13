// written by Ido Avraham : 208699181
// EMAIL: idoavraham086@gmail.com

#include "Board.hpp"
#include "CardDeck.hpp"
#include "Catan.hpp"
#include "Catan_API.hpp"
#include "DevelopmentCard.hpp"
#include "Player.hpp"
#include "TestPlayer.hpp"
#include "Tile.hpp"
#include "doctest.h"

using namespace mycatan;
using namespace std;

TEST_CASE("Turns logic ") {
    cout << "Starting test case: Turns logic" << endl;
    // create 3 players
    Player p1("ido");
    Player p2("shoam");
    Player p3("shlomi");

    Catan catan(p1, p2, p3);

    SUBCASE("Starting player index is within valid range") {
        cout << "Running sub-case: Starting player index is within valid range" << endl;

        size_t startingPlayerIndex = catan.chooseStartingPlayer();
        CHECK((0 <= startingPlayerIndex && startingPlayerIndex <= 2));
    }

    SUBCASE("Player turn is correctly set") {


        size_t startingPlayerIndex = catan.chooseStartingPlayer();
        CHECK((0 <= startingPlayerIndex && startingPlayerIndex <= 2));
        // Check if the correct player turn is set
        CHECK((p1.getMyTurn() ==
               (startingPlayerIndex == 0)));  // p1's turn is set to true if and only if startingPlayerIndex is 0.
        CHECK((p2.getMyTurn() == (startingPlayerIndex == 1)));
        CHECK((p3.getMyTurn() == (startingPlayerIndex == 2)));
    }

    SUBCASE("Other players are added correctly") {
        catan.chooseStartingPlayer();
        auto players = catan.getPlayers();

        CHECK((p1.getOtherPlayers().size() == 2));
        CHECK(p1.getOtherPlayers()[NEXT_PLAYER]->getName() == "shoam");
        CHECK(p1.getOtherPlayers()[BEFORE_PLAYER]->getName() == "shlomi");

        CHECK((p2.getOtherPlayers().size() == 2));
        CHECK((p2.getOtherPlayers()[NEXT_PLAYER]->getName() == "shlomi"));
        CHECK((p2.getOtherPlayers()[BEFORE_PLAYER]->getName() == "ido"));

        CHECK((p3.getOtherPlayers().size() == 2));
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

TEST_CASE("Dice logic") {
    // init steps
    // create 3 players
    Player p1("ido");
    Player p2("shoam");
    Player p3("shlomi");

    // start the game and get board instance
    Catan catan(p1, p2, p3);
    catan.chooseStartingPlayer();
    Board* board = Board::getInstance();

    SUBCASE("Rolled dice result  is within valid range") {
        size_t startingPlayerIndex = catan.chooseStartingPlayer();
        auto players = catan.getPlayers();

        size_t res = players[startingPlayerIndex]->rollDice();
        CHECK((2 <= res && res <= 12));
    }

    SUBCASE("Test 7 rolled in the dice") {
        // add resources to all the players
        p1.addResource(Resources::Wheat, 6);
        p1.addResource(Resources::Wool, 6);
        p2.addResource(Resources::Brick, 6);
        p2.addResource(Resources::Wool, 6);
        p3.addResource(Resources::Wood, 6);
        p3.addResource(Resources::Ore, 6);

        // set p1 true, roll the dice until the outcome is 7
        p1.setTurn(true);
        bool notSeven = true;
        while (notSeven) {
            size_t res = p1.rollDice();
            if (res == 7)
                notSeven = false;
        }

        // Verify that each player decreased half of their resources
        CHECK((TestPlayer::getResourceCount(p1, Resources::Wheat) == 3));
        CHECK((TestPlayer::getResourceCount(p1, Resources::Wool) == 3));
        CHECK((TestPlayer::getResourceCount(p2, Resources::Brick) == 3));
        CHECK((TestPlayer::getResourceCount(p2, Resources::Wool) == 3));
        CHECK((TestPlayer::getResourceCount(p3, Resources::Wood) == 3));
        CHECK((TestPlayer::getResourceCount(p3, Resources::Ore) == 3));
    }

    SUBCASE("Test resources allocation according to the dice rolled") {
        // Place initial settlements and roads for setup
        p1.setTurn(true);
        p1.PlaceFirstSettlements(2, 0, 3, 0);  // Tile (2,0)- Resource: ore, numToken: 10

        p2.setTurn(true);
        p2.PlaceFirstSettlements(2, 5, 3, 5);  // Tile (0,4) - Resource: brick, numToken: 5

        p3.setTurn(true);
        p3.PlaceFirstSettlements(8, 2, 9, 1);  // Tile (4,1) - Resource :brick, numToken: 10

        // Upgrade a settlement of p1 to a city
        p1.addResource(Resources::Wheat, 2);
        p1.addResource(Resources::Ore, 3);
        p1.upgradeToCity(2, 0);

        // Manually set a dice roll that matches the resource allocation for a specific tile
        size_t diceRollForOre = 10;
        size_t diceRollForBrick = 5;

        // Roll the dice for wheat production
        p2.setTurn(true);
        p2.notifyDiceRoll(diceRollForOre);

        // Verify resource allocation for wood, wool; p1 should receive double wood
        // 2 units of Ore from the city, 1 from the settlement , 2 from initial placings
        CHECK((TestPlayer::getResourceCount(p1, Resources::Ore) == 5));

        p1.setTurn(true);
        // Roll the dice for brick production
        p1.notifyDiceRoll(diceRollForBrick);

        // Verify resource allocation for brick p3 received from the settlement at (8,2)
        // 1 unit of brick from each settlement, 2 from inital placings
        CHECK((TestPlayer::getResourceCount(p2, Resources::Brick) == 4));
    }

    //board->cleanup();
}

TEST_CASE("Test cards usage") {
    // create 3 players
    Player p1("ido");
    Player p2("shoam");
    Player p3("shlomi");
    p1.addOthersPlayers(p2, p3);
    // mycatan::Catan catan(p1, p2, p3);

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

        auto* yearOfPlentyCard = new YearOfPlentyCard();
        TestPlayer::addCardToPlayer(p1, yearOfPlentyCard);
        p1.setTurn(true);
        p1.useYearOfPlentyCard(Resources::Brick, Resources::Wheat);
        CHECK((TestPlayer::getResourceCount(p1, Resources::Brick) == 1));
        CHECK((TestPlayer::getResourceCount(p1, Resources::Wheat) == 1));
        CHECK(!p1.getMyTurn());
    }

    SUBCASE("Test Winning Points card ") {
        cout << "Running sub-case: Winning points card" << endl;
        /**
         * This test case verifies the functionality of the Winning Points card.
         *
         * Steps:
         * 1. Add a Winning Points card to player p1.
         * 4. Verify that p1's winning points are updated correctly.
         */

        auto* winningPointsCard = new mycatan::WinningPointsCard();
        TestPlayer::addCardToPlayer(p1, winningPointsCard);
        CHECK((p1.getWinningPoints() == 1));
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
        cout << "Running sub-case: Monopoly card" << endl;

        auto* monopolyCard = new mycatan::MonopolyCard();
        TestPlayer::addCardToPlayer(p1, monopolyCard);

        // Simulate resource addition to other players
        TestPlayer::addResources(p2, Resources::Brick, 5);
        TestPlayer::addResources(p3, Resources::Brick, 3);

        p1.setTurn(true);
        p1.useMonopolyCard(Resources::Brick);

        // Check if p1 collected the correct number of bricks from p2 and p3
        CHECK((TestPlayer::getResourceCount(p1, Resources::Brick) == 8));
        // Check that the bricks decreased from p2 and p3
        CHECK((TestPlayer::getResourceCount(p2, Resources::Brick) == 0));
        CHECK((TestPlayer::getResourceCount(p3, Resources::Brick) == 0));
        // Check that p1 ended his turn after using the card
        CHECK(!p1.getMyTurn());
    }

    SUBCASE("Test road card usage") {
        cout << "Running sub-case: Road card usage" << endl;
        // get instance of the board
        Board* board = Board::getInstance();
        // add to p1 road card
        auto* roadCard = new RoadCard();
        TestPlayer::addCardToPlayer(p1, roadCard);

        // place settlements before placing road
        p1.setTurn(true);
        p1.PlaceFirstSettlements(6, 1, 7, 1);
        // now p1 may use the road card
        p1.useRoadCard(6, 1, 6, 0, 7, 1, 8, 1);

        // get the actual roads from the board
        Vertex* p1_FirstSettlement = board->getVertex(6, 1);
        Vertex* p1_SecondSettlement = board->getVertex(7, 1);

        Vertex* firstRoadSecondVertex = board->getVertex(6, 0);
        Vertex* secondRoadSecondVertex = board->getVertex(8, 1);

        Edge* p1_FirstRoad = board->getEdge(p1_FirstSettlement, firstRoadSecondVertex);
        Edge* p1_SecondRoad = board->getEdge(p1_SecondSettlement, secondRoadSecondVertex);

        // verify that p1 is the owner of the roads
        CHECK((p1_FirstRoad->getOwner() == &p1));
        CHECK((p1_SecondRoad->getOwner() == &p1));

        // verif that other player cant place road on p1 new road
        CHECK(!board->canPlaceRoad(&p2, p1_FirstSettlement, firstRoadSecondVertex));
        CHECK(!board->canPlaceRoad(&p2, p1_SecondSettlement, secondRoadSecondVertex));
    }

    SUBCASE("Test BiggestArmyCard") {
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
        cout << "Running sub-case: Biggest army card" << endl;
        // create 3 knight cards
        auto* firstKnightCard = new KnightCard();
        auto* secondKnightCard = new KnightCard();
        auto* thirdKnightCard = new KnightCard();

        // add the cards to p1
        TestPlayer::addCardToPlayer(p1, firstKnightCard);
        TestPlayer::addCardToPlayer(p1, secondKnightCard);
        TestPlayer::addCardToPlayer(p1, thirdKnightCard);

        // set p1 turn to true, use the card
        p1.setTurn(true);
        p1.getBiggestArmyCard();

        // verify that p1 own 4 cards, and 3 knights , gained 2 winning points
        CHECK((p1.getOwnedCards().size() == 4));
        CHECK((p1.getMyTurn() == false));
        CHECK((p1.getWinningPoints() == 2));

    }

    SUBCASE("Test BiggestArmyCard lost ") {
        cout << "Running sub-case: Biggest army card lost" << endl;
        // Create 3 knight cards
        auto* firstKnightCard = new KnightCard();
        auto* secondKnightCard = new KnightCard();
        auto* thirdKnightCard = new KnightCard();

        // Add the cards to p1
        TestPlayer::addCardToPlayer(p1, firstKnightCard);
        TestPlayer::addCardToPlayer(p1, secondKnightCard);
        TestPlayer::addCardToPlayer(p1, thirdKnightCard);

        // Set p1 turn to true and obtain the Biggest Army card
        p1.setTurn(true);
        p1.getBiggestArmyCard();

        // Verify p1 owns the Biggest Army card and has the correct points
        CHECK(TestPlayer::isOwningBiggestArmyCard(p1));
        CHECK((p1.getWinningPoints() == 2));

        // Create and add a Monopoly card to p2
        auto* monopolyCard = new MonopolyCard();
        TestPlayer::addCardToPlayer(p2, monopolyCard);

        // Now p2 trades a Monopoly card for a Knight card with p1
        p2.setTurn(true);
        p2.tradeDevelopmentCards(&p1, "Knight", "Monopoly");

        // Verify that p1 lost the Biggest Army card and two points, and p2 owns a Knight card
        CHECK(!TestPlayer::isOwningBiggestArmyCard(p1));
        CHECK((p1.getWinningPoints() == 0));
        CHECK((p2.getKnightCount() == 1));
        CHECK(!TestPlayer::isOwningBiggestArmyCard(p1));
        CHECK((p1.getWinningPoints() == 0));

    }
}

TEST_CASE("Test card Deck ") {
    SUBCASE("Player buys development card") {
        /**
         * This test case verifies the functionality of the CardDeck and the ability of a player to buy a development
         * card.
         *
         * Steps:
         * 1. Initialize the CardDeck.
         * 2. Draw a card and verify its type and that it is correctly removed from the deck.
         * 3. Create a player and ensure they can buy a development card if they have enough resources and it's their
         * turn.
         * 4. Verify that the card is added to the player's owned cards and that the player's resources are decremented
         * correctly.
         */
        cout << "Running sub-case: buy dev card" << endl;

        // Create a player with sufficient resources
        Player p1("ido");
        p1.setTurn(true);

        // get the deck instance
        CardDeck* deck = CardDeck::getInstance();

        // Ensure the player has enough resources
        TestPlayer::addResources(p1, Resources::Ore, 1);
        TestPlayer::addResources(p1, Resources::Wheat, 1);
        TestPlayer::addResources(p1, Resources::Wool, 1);

        // Save the original deck size
        size_t initialDeckSize = CardDeck::getDeckSize();

        // Buy a development card
        p1.buyDevelopmentCard();
        // get the new deck size after p1 bought a card
        size_t newDeckSize = mycatan::CardDeck::getDeckSize();

        // Ensure the deck size decreases by 1
        CHECK((newDeckSize == initialDeckSize - 1));

        // Verify that the player owns one more card
        CHECK((p1.getOwnedCards().size() == 1));

        // Verify the player's resources are decremented correctly
        CHECK((TestPlayer::getResourceCount(p1, Resources::Wheat) == 0));
        CHECK((TestPlayer::getResourceCount(p1, Resources::Ore) == 0));
        CHECK((TestPlayer::getResourceCount(p1, Resources::Wool) == 0));

        // Verify the card's owner is set correctly
        Card* ownedCard = p1.getOwnedCards().back();
        CHECK(ownedCard->getOwner() == p1.getName());

        // Cleanup the deck and the drawn card
        deck->cleanup();
    }

    SUBCASE("Draw all the cards from the deck") {
        /**
         * This subcase verifies that all deck can be drawn from the deck and the deck becomes empty.
         *
         * Steps:
         * 1. Continuously draw deck until the deck is empty.
         * 2. Verify that the correct number of deck are drawn.
         * 3. Verify that the deck is empty after drawing all deck.
         * 4. Attempt to draw another card once the deck is empty.
         */
        cout << "Running sub-case: Draw all cards" << endl;

        // init the deck
        CardDeck* deck = CardDeck::getInstance();
        size_t initialDeckSize = CardDeck::getDeckSize();
        std::vector<mycatan::Card*> drawnCards;

        // Draw all deck
        for (size_t i = 0; i < initialDeckSize; ++i) {
            mycatan::Card* card = CardDeck::drawCard();
            REQUIRE((card != nullptr));
            drawnCards.push_back(card);
        }

        // Verify that the correct number of deck is drawn
        CHECK((drawnCards.size() == initialDeckSize));

        // Verify that the deck is empty
        size_t afterAllCardDrawedSize = CardDeck::getDeckSize();
        CHECK((afterAllCardDrawedSize == 0));

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

        deck->cleanup();
    }
}

TEST_CASE("Test trade") {
    Player p1("ido");
    Player p2("shoam");

    SUBCASE("Test valid resources trade") {
        // Add the players enough resources to trade
        TestPlayer::addResources(p1, Resources::Wheat, 4);
        TestPlayer::addResources(p2, Resources::Wood, 5);

        p1.setTurn(true);
        p1.tradeResources(&p2, Resources::Wood, Resources::Wheat, 2, 3);

        // verify that the resources amount updated according to the trade preformed by the players
        CHECK((mycatan::TestPlayer::getResourceCount(p1, Resources::Wood) == 2));
        CHECK((mycatan::TestPlayer::getResourceCount(p2, Resources::Wheat) == 3));
    }

    SUBCASE("Test inValid resources trade") {
        // Scenario: Player 1 doesn't have enough resources to trade
        CHECK_THROWS(p1.tradeResources(&p2, Resources::Wood, Resources::Wheat, 6, 3));
        // Scenario: Player 2 doesn't have enough resources to trade
        CHECK_THROWS(p1.tradeResources(&p2, Resources::Wood, Resources::Wheat, 2, 6));
        // Scenario: Player 1 wants to trade more resources than they have
        mycatan::TestPlayer::addResources(p1, Resources::Wheat, 2);
        CHECK_THROWS(p1.tradeResources(&p2, Resources::Wheat, Resources::Wood, 3, 2));
    }

    SUBCASE("Test valid card trade") {
        // add to the player card to trade for
        auto* yearOfPlentyCard = new mycatan::YearOfPlentyCard();
        mycatan::TestPlayer::addCardToPlayer(p1, yearOfPlentyCard);

        auto* monopolyCard = new mycatan::MonopolyCard();
        mycatan::TestPlayer::addCardToPlayer(p2, monopolyCard);

        p1.setTurn(true);
        p1.tradeDevelopmentCards(&p2, "Monopoly", "Year of Plenty");

        // verify that both players have 1 owned card
        CHECK((p1.getOwnedCards().size() == 1));
        CHECK((p2.getOwnedCards().size() == 1));

        // verify that monopoly card owner is ido and vice versa
        CHECK(monopolyCard->getOwner() == "ido");
        CHECK(yearOfPlentyCard->getOwner() == "shoam");

        // clean up
        //delete yearOfPlentyCard;
        //delete monopolyCard;
    }

    SUBCASE("Test inValid card trade") {
        // Scenario: Player 1 doesn't own the specified card to trade
        CHECK_THROWS(p1.tradeDevelopmentCards(&p2, "SomeNonexistentCard", "Year of Plenty"));
        // Scenario: Player 2 doesn't own the specified card to trade
        CHECK_THROWS(p1.tradeDevelopmentCards(&p2, "Monopoly", "SomeNonexistentCard"));
    }
}

TEST_CASE("Test Board") {
    // Initialize the board
    mycatan::Board* board = mycatan::Board::getInstance();

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
            std::vector<std::pair<size_t, size_t>> expectedVertices = {{u, v},         {u + 1, v},     {u + 2, v},
                                                                       {u + 2, v + 1}, {u + 1, v + 1}, {u, v + 1}};

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
        board->cleanup();
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
                    {Board::getInstance()->getVertex(u, v + 1), Board::getInstance()->getVertex(u, v)}};

            std::vector<Edge*> edges = tile->getEdges();
            REQUIRE((edges.size() == 6));

            for (const auto& expectedEdge : expectedEdges) {
                Vertex* v1 = expectedEdge.first;
                Vertex* v2 = expectedEdge.second;
                bool edgeFound = false;

                for (Edge* edge : edges) {
                    if ((edge->getVertex1() == v1 && edge->getVertex2() == v2) ||
                        (edge->getVertex1() == v2 && edge->getVertex2() == v1)) {
                        edgeFound = true;
                        break;
                    }
                }
                CHECK(edgeFound);
            }
        }
    }

   // board->cleanup();
}

TEST_CASE("Test Place first Road and Settlement") {
    // Initial setup
    // Create 3 players
    Player p1("ido");
    Player p2("shoam");
    Player p3("shlomi");

    // Initialize the board
    Board* board = Board::getInstance();

    WHEN("Placing the first settlements") {
        p1.setTurn(true);
        p1.PlaceFirstSettlements(6, 1, 8, 0);

        Vertex* p1_FirstSettlement = board->getVertex(8, 0);
        Vertex* p1_SecondSettlement = board->getVertex(6, 1);

        THEN("The settlements should be correctly placed") {
            CHECK((p1_FirstSettlement->getOwner() == &p1));
            CHECK((p1_SecondSettlement->getOwner() == &p1));
            // Another player can't place settlement in the same spot
            CHECK(!board->canPlaceSettlement(&p2, p1_FirstSettlement));
            CHECK(!board->canPlaceSettlement(&p2, p1_SecondSettlement));

            AND_THEN("Placing the first roads should work correctly") {
                // Place two first Roads
                p1.placeFirstRoads(6, 1, 6, 0, 8, 0, 7, 0);

                Vertex* firstRoadSecondVertex = board->getVertex(7, 0);
                Vertex* secondRoadSecondVertex = board->getVertex(6, 0);

                Edge* p1_FirstRoad = board->getEdge(p1_FirstSettlement, firstRoadSecondVertex);
                Edge* p1_SecondRoad = board->getEdge(p1_SecondSettlement, secondRoadSecondVertex);

                CHECK((p1_FirstRoad->getOwner() == &p1));
                CHECK((p1_SecondRoad->getOwner() == &p1));

                CHECK(!board->canPlaceRoad(&p2, p1_FirstSettlement, firstRoadSecondVertex));
                CHECK(!board->canPlaceRoad(&p2, p1_SecondSettlement, secondRoadSecondVertex));

                AND_THEN("p1 should have received the required amount of each resource and correct winning points") {
                    // Get each settlement's associated Tiles
                    std::vector<Tile*> firstSettlementTiles = board->getAdjacentTiles(p1_FirstSettlement);
                    std::vector<Tile*> secondSettlementTiles = board->getAdjacentTiles(p1_SecondSettlement);

                    // Check that p1 has the right amount of resources
                    std::vector<size_t> resourcesRequiredCount(5, 0);
                    for (Tile* tile : firstSettlementTiles) {
                        size_t resourceIndex = resourceToInt(tile->getResourceType());
                        resourcesRequiredCount[resourceIndex]++;
                    }
                    for (Tile* tile : secondSettlementTiles) {
                        size_t resourceIndex = resourceToInt(tile->getResourceType());
                        resourcesRequiredCount[resourceIndex]++;
                    }

                    // Verify that p1 received the required amount of each resource
                    size_t actualBrickAmount = TestPlayer::getResourceCount(p1, Resources::Brick);
                    CHECK((actualBrickAmount == resourcesRequiredCount[BRICK]));

                    size_t actualWheatAmount = TestPlayer::getResourceCount(p1, Resources::Wheat);
                    CHECK((actualWheatAmount == resourcesRequiredCount[WHEAT]));

                    size_t actualWoolAmount = TestPlayer::getResourceCount(p1, Resources::Wool);
                    CHECK((actualWoolAmount == resourcesRequiredCount[WOOL]));

                    size_t actualWoodAmount = TestPlayer::getResourceCount(p1, Resources::Wood);
                    CHECK((actualWoodAmount == resourcesRequiredCount[WOOD]));

                    size_t actualOreAmount = TestPlayer::getResourceCount(p1, Resources::Ore);
                    CHECK((actualOreAmount == resourcesRequiredCount[ORE]));

                    // Verify that p1 has the correct winning points
                    CHECK((p1.getWinningPoints() == 2));
                }
            }
        }
    }

    // Cleanup the board after tests
    board->cleanup();
}

TEST_CASE("Test Place Regular Settlements and Roads") {
    // Initial setup
    // Create 3 players
    Player p1("ido");
    Player p2("shoam");
    Player p3("shlomi");

    // Initialize the game and board
    Catan catan(p1, p2, p3);
    Board* board = Board::getInstance();

    // Place initial settlements and roads for setup
    p1.setTurn(true);
    p1.PlaceFirstSettlements(6, 1, 8, 0);
    p1.placeFirstRoads(6, 1, 6, 0, 8, 0, 7, 0);

    p2.setTurn(true);
    p2.PlaceFirstSettlements(5, 2, 7, 1);
    p2.placeFirstRoads(5, 2, 5, 1, 7, 1, 6, 1);

    p3.setTurn(true);
    p3.PlaceFirstSettlements(8, 2, 9, 1);
    p3.placeFirstRoads(8, 2, 9, 2, 9, 1, 8, 1);

    WHEN("Placing a regular settlement and road") {
        // Manually add resources to p1
        p1.addResource(Resources::Wood, 1);
        p1.addResource(Resources::Brick, 1);
        p1.addResource(Resources::Wheat, 1);
        p1.addResource(Resources::Wool, 1);

        // Set p1's turn
        p1.setTurn(true);

        // Place a settlement at a new location
        p1.placeSettlement(7, 0);

        Vertex* newSettlementVertex = board->getVertex(7, 0);
        THEN("The settlement should be correctly placed") {
            CHECK((newSettlementVertex->getOwner() == &p1));
        }

        AND_WHEN("Placing a road connected to the new settlement") {
            // Manually add resources to p1
            p1.addResource(Resources::Wood, 1);
            p1.addResource(Resources::Brick, 1);

            // Place a road connected to the new settlement
            p1.placeRoad(7, 0, 6, 0);

            Vertex* roadVertex1 = board->getVertex(6, 0);
            Vertex* roadVertex2 = board->getVertex(7, 0);
            Edge* newRoad = board->getEdge(roadVertex1, roadVertex2);

            THEN("The road should be correctly placed") {
                CHECK((newRoad->getOwner() == &p1));
            }
        }
    }

    // Cleanup the board after tests
   // board->cleanup();
}

TEST_CASE("Test upgrade settlement to a city ") {
    // Initial setup
    // Create 3 players
    Player p1("ido");
    Player p2("shoam");
    Player p3("shlomi");

    // Initialize the game and board
    Catan catan(p1, p2, p3);
    Board* board = Board::getInstance();

    // Place initial settlements and roads for setup
    p1.setTurn(true);
    p1.PlaceFirstSettlements(6, 1, 8, 0);
    p1.placeFirstRoads(6, 1, 6, 0, 8, 0, 7, 0);

    SUBCASE("valid settlement upgrade") {
        // give p1 enough resources to upgrade the settlement into a city
        p1.addResource(Resources::Wheat, 2);
        p1.addResource(Resources::Ore, 3);

        // try to upgrade the settlement at 6,1 into a city
        p1.upgradeToCity(6, 1);
        // verify that p1 is the owner of the city, and that the vertex is now a city
        Vertex* newCity = board->getVertex(6, 1);
        CHECK(newCity->isCity());
        CHECK((newCity->getOwner() == &p1));
    }
    SUBCASE("invalid settlement upgrade") {
        // try to upgrade the settlement at 6,1 into a city without enough resources
        CHECK_THROWS(p1.upgradeToCity(6, 1));
    }

    // clean up
    board->cleanup();
}

TEST_CASE("Invalid Moves and Error Handling") {
    // Create 3 players
    Player p1("ido");
    Player p2("shoam");
    Player p3("shlomi");

    // Initialize the game and board
    Catan catan(p1, p2, p3);
    Board* board = Board::getInstance();
    CardDeck* deck = CardDeck::getInstance();

    SUBCASE("Invalid Settlement Placement") {
        p1.setTurn(true);
        p1.PlaceFirstSettlements(6, 1, 8, 0);
        // Try to place a settlement on an occupied vertex
        CHECK_THROWS_AS(p1.placeSettlement(8, 0), std::runtime_error);

        // Try to place a settlement without a connecting road
        CHECK_THROWS_AS(p1.placeSettlement(5, 5), std::runtime_error);
    }

    SUBCASE("Invalid Road Placement") {
        p1.setTurn(true);
        p1.PlaceFirstSettlements(6, 1, 8, 0);
        p1.placeFirstRoads(6, 1, 6, 0, 8, 0, 7, 0);

        // Try to place a road without a connecting settlement or road
        CHECK_THROWS_AS(p1.placeRoad(5, 5, 5, 4), std::runtime_error);

        // Try to place a road on an occupied edge
        CHECK_THROWS_AS(p1.placeRoad(6, 1, 6, 0), std::runtime_error);
    }

    SUBCASE("Invalid Resource Transactions") {
        p1.setTurn(true);
        p1.addResource(Resources::Wood, 1);

        // Try to trade more resources than available
        CHECK_THROWS_AS(p1.tradeResources(&p2, Resources::Wheat, Resources::Wood, 2, 1), std::runtime_error);

        // Try to buy a development card without enough resources
        CHECK_THROWS_AS(p1.buyDevelopmentCard(), std::runtime_error);
    }

    SUBCASE("Invalid Development Card Usage") {
        auto* yearOfPlentyCard = new YearOfPlentyCard();
        TestPlayer::addCardToPlayer(p1, yearOfPlentyCard);

        // Try to use the card out of turn
        CHECK_THROWS_AS(p2.useYearOfPlentyCard(Resources::Brick, Resources::Wheat), std::runtime_error);

        // Use the card in turn
        p1.setTurn(true);
        p1.useYearOfPlentyCard(Resources::Brick, Resources::Wheat);
        CHECK(!p1.getMyTurn());  // Ensure turn ends after use

        // Try to use the card again in the same turn
        CHECK_THROWS_AS(p1.useYearOfPlentyCard(Resources::Brick, Resources::Wheat), std::runtime_error);
    }

    SUBCASE("Invalid Upgrade to City") {
        p1.setTurn(true);
        p1.PlaceFirstSettlements(6, 1, 8, 0);

        // Try to upgrade without enough resources
        CHECK_THROWS_AS(p1.upgradeToCity(6, 1), std::runtime_error);

        // Add enough resources and upgrade
        p1.addResource(Resources::Wheat, 2);
        p1.addResource(Resources::Ore, 3);
        p1.upgradeToCity(6, 1);
        Vertex* upgradedVertex = board->getVertex(6, 1);
        CHECK(upgradedVertex->isCity());

        // Try to upgrade the same settlement again
        CHECK_THROWS_AS(p1.upgradeToCity(6, 1), std::runtime_error);

        // Try to upgrade a non-existent settlement
        CHECK_THROWS_AS(p1.upgradeToCity(5, 5), std::runtime_error);
    }

    // clean-ups
   // board->cleanup();
    //deck->cleanup();
}

SCENARIO("Valid game round") {
    cout << "************************VALID GAME ROUND ***********************************" << endl;
    // Initial setup: Create 3 players and initialize the game and board
    Player p1("ido");
    Player p2("shoam");
    Player p3("shlomi");
    Catan catan(p1, p2, p3);
    Board* board = Board::getInstance();

    GIVEN("Three players have been initialized and the game has started") {
        // Choose the starting player
        size_t startingPlayerIndex = catan.chooseStartingPlayer();
        auto players = catan.getPlayers();

        // Place initial settlements and roads for each player
        p1.PlaceFirstSettlements(6, 1, 8, 0);
        p1.placeFirstRoads(6, 1, 6, 0, 8, 0, 7, 0);
        p1.endTurn();

        p2.PlaceFirstSettlements(5, 2, 7, 1);
        p2.placeFirstRoads(5, 2, 5, 1, 7, 1, 6, 1);
        p2.endTurn();

        p3.PlaceFirstSettlements(8, 2, 9, 1);
        p3.placeFirstRoads(8, 2, 9, 2, 9, 1, 8, 1);
        p3.endTurn();

        WHEN("The first player places another settlement, rolls the dice, and ends their turn") {
            // The First player places another settlement and road
            p1.setTurn(true);
            p1.addResource(Resources::Wood, 1);
            p1.addResource(Resources::Brick, 1);
            p1.addResource(Resources::Wheat, 1);
            p1.addResource(Resources::Wool, 1);
            p1.placeSettlement(7, 0);

            p1.addResource(Resources::Wood, 1);
            p1.addResource(Resources::Brick, 1);
            p1.placeRoad(6, 0, 7, 0);

            // First player rolls the dice
             p1.rollDice();
            // The First player ends their turn
            p1.endTurn();

            THEN("The next player buys a development card") {
                size_t nextPlayerIndex = (startingPlayerIndex + 1) % players.size();
                Player* nextPlayer = players[nextPlayerIndex];
                players[nextPlayerIndex]->setTurn(true);

                // Ensure the player has enough resources to buy a development card
                nextPlayer->addResource(Resources::Wheat, 1);
                nextPlayer->addResource(Resources::Ore, 1);
                nextPlayer->addResource(Resources::Wool, 1);

                // Player buys a development card
                nextPlayer->buyDevelopmentCard();

                // Verify that the player owns one more card
                CHECK((nextPlayer->getOwnedCards().size() == 1));

                nextPlayer->endTurn();

                THEN("The last player (p3) places a road") {
                    size_t lastPlayerIndex = (nextPlayerIndex + 1) % players.size();
                    players[lastPlayerIndex]->setTurn(true);

                    // Ensure the player has enough resources to place a road
                    players[lastPlayerIndex]->addResource(Resources::Wood, 1);
                    players[lastPlayerIndex]->addResource(Resources::Brick, 1);

                    // Player places a road
                    players[lastPlayerIndex]->placeRoad(8, 2, 8, 3);
                    players[lastPlayerIndex]->endTurn();

                    THEN("The first player trades resources with another player") {
                        // Ensure the players have enough resources to trade
                        p1.addResource(Resources::Wheat, 3);
                        p2.addResource(Resources::Wood, 2);

                        // Save the starting amount of each player
                        size_t p1StartingWheatAmount = TestPlayer::getResourceCount(p1, Resources::Wheat);
                        size_t p1StartingWoodAmount = TestPlayer::getResourceCount(p1, Resources::Wood);

                        size_t p2StartingWoodAmount = TestPlayer::getResourceCount(p2, Resources::Wood);
                        size_t p2StartingWheatAmount = TestPlayer::getResourceCount(p2, Resources::Wheat);

                        // Player 1 trades resources with Player 2
                        p1.tradeResources(&p2, Resources::Wood, Resources::Wheat, 2, 3);
                        p1.endTurn();

                        // Verify the trade
                        size_t p1CurrentWheatAmount = TestPlayer::getResourceCount(p1, Resources::Wheat);
                        size_t p1CurrentWoodAmount = TestPlayer::getResourceCount(p1, Resources::Wood);
                        size_t p2CurrentWoodAmount = TestPlayer::getResourceCount(p2, Resources::Wood);
                        size_t p2CurrentWheatAmount = TestPlayer::getResourceCount(p2, Resources::Wheat);

                        // Check the trade results
                        CHECK( (p1CurrentWheatAmount == p1StartingWheatAmount - 3));
                        CHECK((p1CurrentWoodAmount == p1StartingWoodAmount + 2));
                        CHECK((p2CurrentWoodAmount == p2StartingWoodAmount - 2));
                        CHECK((p2CurrentWheatAmount == p2StartingWheatAmount + 3));

                        // Verify that the turn goes back to the next player
                        CHECK(players[nextPlayerIndex]->getMyTurn());

                        THEN("Manually add points to the first player to reach winning condition and end the game") {
                            // Manually add points to p1
                            TestPlayer::addWinningPoints(p1, 10);

                            // Check if the game recognizes the winner
                            CHECK((catan.thereIsWinner() == true));
                            CHECK((p1.getWinningPoints() >= 10));
                        }
                    }
                }
            }
        }
    }

    cout << "************************END OF VALID GAME ROUND ***********************************" << endl;
}