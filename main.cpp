#include "Board.hpp"
#include "CardDeck.hpp"
#include "Catan.hpp"
#include "DevelopmentCard.hpp"
#include "TestPlayer.hpp"
#include "Player.hpp"
#include <iostream>

using namespace mycatan;
using namespace std;
/*
int main() {

    // create 3 players
    Player p1("ido");
    Player p2("shoam");
    Player p3("shlomi");
    Catan catan(p1, p2, p3);


    //p1 is always the starting player (can be changed easily in Catan cpp)
    catan.chooseStartingPlayer();

    // Place initial settlements and roads for each player
    // p1: Settlements on tiles with resources: wheat, wood, wool, ore
    p1.PlaceFirstSettlements(7, 3, 8,3); // now p1 should have: 1-wheat, 2-wood, 2-wool, 1-ore
    p1.placeFirstRoads(7, 3, 8,3 , 8, 3, 9, 3);
    p1.endTurn();

    // p2: Settlements on tiles with resources: Brick (6) ,  ore (10) , wheat(12) , wool (2 ) , wool (4) , wood(9)
    p2.PlaceFirstSettlements(3, 1, 6, 1); // now p2 should have: 1-wheat, 1-brick, 1-ore, 2-wool, 1-wood
    p2.placeFirstRoads(6, 1, 6, 0, 3, 1, 4, 1);
    p2.endTurn();

    // p3: Settlements on tiles with resources: Brick (10), Ore (8), Wood (9)
    p3.PlaceFirstSettlements(8, 2, 9, 1);
    p3.placeFirstRoads(8, 2, 9, 2, 9, 1, 8, 1);
    p3.endTurn();


    // Player 1 (p1) buys a development card
    p1.buyDevelopmentCard();
    p1.endTurn();

    // Player 2 (p2) builds another settlement and trades with Player 3 (p3), then rolls the dice
    p2.placeSettlement(6, 0);
    p2.tradeResources(&p3, Resources::Brick, Resources::Ore, 1, 1);
    p2.rollDice();
    p2.endTurn();

    // Player 3 (p3) builds a road
    p3.placeRoad(8, 2, 7, 2);
    p3.rollDice();
    p3.endTurn();

    catan.thereIsWinner();


    // make a phony winner to end the game properly
    cout<<"************PHONY WINNER******************"<<endl;
    TestPlayer::addWinningPoints(p1,8);
    catan.thereIsWinner();

    return 0;
}
*/
