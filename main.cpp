#include "Board.hpp"
#include "CardDeck.hpp"
#include "Catan.hpp"
#include "DevelopmentCard.hpp"
#include "Player.hpp"
#include <iostream>

using namespace mycatan;
using namespace std;

int main() {

    // create 3 players
    Player p1("ido");
    Player p2("shoam");
    Player p3("shlomi");
    Catan catan(p1, p2, p3);


    //p1 is always the starting player (can be changed easily in Catan cpp)
    catan.chooseStartingPlayer();

    // Place initial settlements and roads for each player
    // p1: Settlements on tiles with resources: Brick, Wheat, Wool
    p1.PlaceFirstSettlements(0, 4, 1, 4); // Brick (5), Wheat (6)
    p1.placeFirstRoads(0, 4, 1, 4, 1, 4, 2, 4);
    p1.endTurn();

    // p2: Settlements on tiles with resources: Wool, Wood, Wheat
    p2.PlaceFirstSettlements(2, 3, 3, 3); // Wool (11), Wood (8), Wheat (4)
    p2.placeFirstRoads(2, 3, 2, 2, 3, 3, 3, 2);
    p2.endTurn();

    // p3: Settlements on tiles with resources: Brick, Ore, Wood
    p3.PlaceFirstSettlements(4, 1, 4, 0); // Brick (10), Ore (8), Wood (9)
    p3.placeFirstRoads(4, 1, 4, 0, 3, 0, 3, 1);
    p3.endTurn();

    // Player 1 (p1) buys a development card
    p1.buyDevelopmentCard();
    p1.endTurn();

    // Player 2 (p2) builds another settlement and trades with Player 3 (p3), then rolls the dice
    p2.placeSettlement(3, 1); // Assume valid placement
    p2.tradeResources(&p3, Resources::Wood, Resources::Wheat, 2, 3);
    p2.rollDice();
    p2.endTurn();

    // Player 3 (p3) builds a road
    p3.placeRoad(4, 1, 3, 1);
    p3.endTurn();

    return 0;
}

