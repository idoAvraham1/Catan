//written by Ido Avraham : 208699181
//EMAIL: idoavraham086@gmail.com

#include "Catan_API.hpp"
#include "doctest.h"
#include "Catan.hpp"
#include "Player.hpp"
#include "TestPlayer.hpp"
#include "DevelopmentCard.hpp"

using namespace std;

TEST_CASE("Turns logic ") {

    // create 3 players
    mycatan::Player p1("ido");
    mycatan::Player p2("shoam");
    mycatan::Player p3("shlomi");

    mycatan::Catan catan(p1, p2, p3);

    SUBCASE("Starting player index is within valid range") {
        size_t startingPlayerIndex = catan.chooseStartingPlayer();
        CHECK(( 0 <= startingPlayerIndex && startingPlayerIndex <= 2));
    }

    SUBCASE("Player turn is correctly set") {
        size_t startingPlayerIndex = catan.chooseStartingPlayer();
        CHECK(( 0<= startingPlayerIndex && startingPlayerIndex <= 2));
        // Check if the correct player turn is set
        CHECK(p1.getMyTurn() ==(startingPlayerIndex == 0)); // p1's turn is set to true if and only if startingPlayerIndex is 0.
        CHECK(p2.getMyTurn() == (startingPlayerIndex == 1));
        CHECK(p3.getMyTurn() == (startingPlayerIndex == 2));
    }

    SUBCASE("Other players are added correctly") {
        catan.chooseStartingPlayer();
        auto players = catan.getPlayers();

        CHECK(p1.getOtherPlayers().size() == 2);
        CHECK(p1.getOtherPlayers()[NEXT_PLAYER]->getName() == "shoam");
        CHECK(p1.getOtherPlayers()[BEFORE_PLAYER]->getName() == "shlomi");

        CHECK(p2.getOtherPlayers().size() == 2);
        CHECK(p2.getOtherPlayers()[NEXT_PLAYER]->getName() == "shlomi");
        CHECK(p2.getOtherPlayers()[BEFORE_PLAYER]->getName() == "ido");

        CHECK(p3.getOtherPlayers().size() == 2);
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
    mycatan::Player p1("ido");
    mycatan::Player p2("shoam");
    mycatan::Player p3("shlomi");

    mycatan::Catan catan(p1, p2, p3);

    SUBCASE("Rolled dice result  is within valid range") {
        size_t startingPlayerIndex = catan.chooseStartingPlayer();
        auto players = catan.getPlayers();

        size_t res = players[startingPlayerIndex]->rollDice();
        CHECK(( 2 <= res && res <= 12 ));
    }

}

TEST_CASE("Test cards usage"){
    // create 3 players
    mycatan::Player p1("ido");
    mycatan::Player p2("shoam");
    mycatan::Player p3("shlomi");
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
         * 4. Verify that the Year of Plenty card is marked as used.
         * 5. Verify that p1's resource counts for Brick and Wheat are updated correctly.
         * 6. Verify that p1's turn ends after using the card.
         */

        auto* yearOfPlentyCard = new mycatan::YearOfPlentyCard();
        mycatan::TestPlayer::addCardToPlayer(p1, yearOfPlentyCard);
        p1.setTurn(true);
        p1.useYearOfPlentyCard(Resources::Brick, Resources::Wheat);
        CHECK(yearOfPlentyCard->isUsed() == true);
        CHECK(mycatan::TestPlayer::getResourceCount(p1, Resources::Brick) == 1);
        CHECK(mycatan::TestPlayer::getResourceCount(p1, Resources::Wheat) == 1);
        CHECK(!p1.getMyTurn());
        delete yearOfPlentyCard;
    }

    SUBCASE("Test Winning Points card usage") {
        /**
         * This test case verifies the functionality of the Winning Points card.
         *
         * Steps:
         * 1. Add a Winning Points card to player p1.
         * 2. Set p1's turn to true.
         * 3. Use the Winning Points card to gain 2 winning points.
         * 4. Verify that p1's winning points are updated correctly.
         * 5. Verify that the Winning Points card is marked as used.
         * 6. Verify that p1's turn ends after using the card.
         */

        auto* winningPointsCard = new mycatan::WinningPointsCard();
        mycatan::TestPlayer::addCardToPlayer(p1, winningPointsCard);
        p1.setTurn(true);
        p1.useWinningPointsCard();
        CHECK(p1.getWinningPoints() == 2);
        CHECK(winningPointsCard->isUsed() == true);
        CHECK(!p1.getMyTurn());
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
         * 6. Verify that the Monopoly card is marked as used.
         * 7. Verify that the Brick resources of p2 and p3 are updated correctly (set to 0).
         * 8. Verify that p1's turn ends after using the card.
         */

        auto* monopolyCard = new mycatan::MonopolyCard();
        mycatan::TestPlayer::addCardToPlayer(p1, monopolyCard);

        // Simulate resource addition to other players
        p2.addResource(Resources::Brick, 5);
        p3.addResource(Resources::Brick, 3);

        p1.setTurn(true);
        p1.useMonopolyCard(Resources::Brick);

        // Check if p1 collected the correct number of bricks from p2 and p3
        CHECK(mycatan::TestPlayer::getResourceCount(p1, Resources::Brick) == 8);
        CHECK(monopolyCard->isUsed() == true);
        // Check that the bricks decreased from p2 and p3
        CHECK(mycatan::TestPlayer::getResourceCount(p2, Resources::Brick) == 0);
        CHECK(mycatan::TestPlayer::getResourceCount(p3, Resources::Brick) == 0);
        // Check that p1 ended his turn after using the card
        CHECK(!p1.getMyTurn());

        delete monopolyCard;
    }

}

TEST_CASE("Test card Deck "){

    SUBCASE("Draw card from deck") {
        /**
            * This test case verifies the functionality of the CardDeck and the ability of a player to buy a development card.
            *
            * Steps:
            * 1. Initialize the CardDeck.
            * 2. Draw a card and verify its type and that it is correctly removed from the deck.
            * 3. Create a player and ensure they can buy a development card if they have enough resources and it's their turn.
            * 4. Verify that the card is added to the player's owned cards and that the player's resources are decremented correctly.
        */
        // Draw a card
        mycatan::Card* drawnCard = mycatan::CardDeck::drawCard();

        // Check that the card is valid
        REQUIRE(drawnCard != nullptr);

        // Ensure the deck size decreases by 1
        size_t initialDeckSize = mycatan::CardDeck::getDeckSize();
        mycatan::CardDeck::drawCard();  // Draw another card
        CHECK(mycatan::CardDeck::getDeckSize() == initialDeckSize - 1);

        // Clean up
        delete drawnCard;
    }

    SUBCASE("Player buys development card") {
        // Create a player with sufficient resources
        mycatan::Player p1("Player 1");
        p1.setTurn(true);

        // Ensure the player has enough resources
        p1.addResource(Resources::Wheat, 1);
        p1.addResource(Resources::Wood, 1);
        p1.addResource(Resources::Wool, 1);

        // Buy a development card
        p1.buyDevelopmentCard();

        // Verify that the player owns one more card
        CHECK(p1.getOwnedCards().size() == 1);

        // Verify the player's resources are decremented correctly
        CHECK(mycatan::TestPlayer::getResourceCount(p1, Resources::Wheat) == 0);
        CHECK(mycatan::TestPlayer::getResourceCount(p1, Resources::Wood) == 0);
        CHECK(mycatan::TestPlayer::getResourceCount(p1, Resources::Wool) == 0);

        // Verify the card's owner is set correctly
        mycatan::Card* ownedCard = p1.getOwnedCards().back();
        CHECK(ownedCard->getOwner() == p1.getName());
    }

    SUBCASE("Draw all the cards from the deck"){
        /**
      * This subcase verifies that all cards can be drawn from the deck and the deck becomes empty.
      *
      * Steps:
      * 1. Continuously draw cards until the deck is empty.
      * 2. Verify that the correct number of cards are drawn.
      * 3. Verify that the deck is empty after drawing all cards.
      * 4. attempt to draw another card once the deck is empty
      */

        size_t initialDeckSize = mycatan::CardDeck::getDeckSize();
        std::vector<mycatan::Card*> drawnCards;

        // Draw all cards
        for (size_t i = 0; i < initialDeckSize; ++i) {
            mycatan::Card* card = mycatan::CardDeck::drawCard();
            REQUIRE(card != nullptr);
            drawnCards.push_back(card);
        }

        // Verify that the correct number of cards is drawn
        CHECK(drawnCards.size() == initialDeckSize);

        // Verify that the deck is empty
        CHECK(mycatan::CardDeck::getDeckSize() == 0);

        // Try to draw another card after the deck is empty and catch the exception
        bool exceptionCaught = false;
        try {
            mycatan::CardDeck::drawCard();
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


