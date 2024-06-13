# Catan Game Implementation

## Overview
This project is an implementation of the board game "Catan" in C++. It simulates the game mechanics, including resource management, development cards, settlements, roads, and cities. The project includes classes to manage the game logic, players, resources, and the board.

![Board Coordinates](path_to_your_image)

### Board Mapping and Coordinates
The board is represented using a predefined tile configuration. Each tile has a resource type and a number token. The vertices of each tile are calculated using the formula:
(where the coordinates of the tile are (x,y)) 

```cpp
size_t u = 2 * (x - 1) + y;
size_t v = y;
```

Vertices for a tile are:
```cpp
v1 =  (u, v);       // Top-Left Vertex
v2 = (u + 1, v);    // Top Vertex
v3 = (u + 2, v);    // Top-Right Vertex
v4 = (u + 2, v + 1); // Bottom-Right Vertex
v5 = (u + 1, v + 1); // Bottom Vertex
v6 = (u, v + 1);     // Bottom-Left Vertex
```
Edges for a tile are:
```cpp
e1 = (v1, v2);
e2 = (v2, v3);
e3 = (v3, v4);
e4 = (v4, v5);
e5 = (v5, v6);
e6 = (v6, v1);
```
## Classes and Methods

### `Catan`
Manages the overall game, including player turns and determining the winner.

- **Catan(Player& p1, Player& p2, Player& p3)**: Initializes the game with three players.
- **size_t chooseStartingPlayer()**: Chooses the starting player randomly.
- **bool thereIsWinner()**: Checks if there is a winner based on the winning points.
- **void endGame(Player* player)**: Ends the game and declares the winner.

### `DevelopmentCardManagement`
Manages the development cards, including buying and using them.

- **static void buyDevelopmentCard(Player* player)**: Allows a player to buy a development card.
- **static void useMonopolyCard(Player* player, Resources giveMeThatResource)**: Uses a Monopoly card to collect resources from other players.
- **static void useYearOfPlentyCard(Player* player, Resources resource1, Resources resource2)**: Uses a Year of Plenty card to gain resources.
- **static void useRoadCard(Player* player, size_t x1, size_t y1, size_t x2, size_t y2, size_t x3, size_t y3, size_t x4, size_t y4)**: Uses a Road Building card to place roads.
- **static void getBiggestArmyCard(Player* player)**: Awards the Biggest Army card if the player has enough knight cards.
- **static void tradeDevelopmentCards(Player* thisPlayer, Player* other, const std::string& cardIn, const std::string& cardOut)**: Trades development cards between two players.
- **static void deleteOwnedCards(Player* player)**: Deletes all owned cards of a player.

### `Player`
Represents a player in the game, including their resources, settlements, and roads.
- **Player(std::string name)**: Creates a player.
- **void endTurn()**: Ends the player's turn.
- **void addOthersPlayers(Player &otherPlayer0, Player &otherPlayer1)**: Adds other players for trading purposes.
- **size_t rollDice()**: Rolls the dice and returns the result.
- **void notifyDiceRoll(size_t diceRoll)**: Notifies the player of the dice roll result.
- **void buyDevelopmentCard()**: Buys a development card.
- **void useMonopolyCard(Resources giveMeThatResource)**: Uses a Monopoly card.
- **void useYearOfPlentyCard(Resources resource1, Resources resource2)**: Uses a Year of Plenty card.
- **void getBiggestArmyCard()**: Claims the Biggest Army card.
- **void tradeDevelopmentCards(Player *other, const std::string &cardIn, const std::string &cardOut)**: Trades development cards with another player.
- **void useRoadCard(size_t x1, size_t y1, size_t x2, size_t y2, size_t x3, size_t y3, size_t x4, size_t y4)**: Uses a Road Building card.
- **void tradeResources(Player *other, Resources resourceIn, Resources resourceOut, size_t inAmount, size_t outAmount)**: Trades resources with another player.
- **void addResource(Resources resource, size_t amount)**: Adds resources to the player's inventory.
- **void placeSettlement(size_t x, size_t y)**: Places a settlement on the board.
- **void placeRoad(size_t x1, size_t y1, size_t x2, size_t y2)**: Places a road on the board.
- **void PlaceFirstSettlements(size_t x1, size_t y1, size_t x2, size_t y2)**: Places the initial settlements.
- **void placeFirstRoads(size_t x1, size_t y1, size_t x2, size_t y2, size_t x3, size_t y3, size_t x4, size_t y4)**: Places the initial roads.
- **void upgradeToCity(size_t x, size_t y)**: Upgrades a settlement to a city.

### `Vertex`
Represents a vertex on the board, which can hold a settlement or city.
- **Vertex(size_t x, size_t y)**: Creates a vertex at the given coordinates.
- **bool hasSettlement() const**: Checks if there is a settlement on the vertex.
- **bool isCity() const**: Checks if there is a city on the vertex.
- **void buildSettlement(Player* newOwner)**: Builds a settlement for the player.
- **void upgradeToCity()**: Upgrades the settlement to a city.

### `Edge`
Represents a road between two vertices.
- **Edge(Vertex* v1, Vertex* v2)**: Creates a road between two vertices.
- **bool hasRoad() const**: Checks if there is a road on the edge.
- **void placeRoad(Player* newOwner)**: Places a road for the player.

### `Board`
Manages the board, including tiles, vertices, and edges.
- **static Board *getInstance()**: Returns the singleton instance of the board.
- **void initializeBoard()**: Initializes the board with predefined tile configurations.
- **void allocateResources(size_t diceRoll)**: Allocates resources to players based on the dice roll.
- **bool canPlaceSettlement(Player *player, Vertex *vertex)**: Checks if a settlement can be placed on a vertex by a player.
- **bool canPlaceRoad(Player *player, Vertex *vertex1, Vertex *vertex2)**: Checks if a road can be placed between two vertices by a player.

### `ResourceManagement`
Manages the resources for the players.
- **static void addResource(Player* player,Resources resource , size_t amount)**: Adds resources to the player.
- **static void decreaseResource(Player* player , Resources resource, size_t amount)**: Decreases resources from the player.
- **static size_t giveAllResourcesOfType(Player* player , Resources resource)**: Gives all resources of a type from the player.
- **static void tradeResources(Player* thisPlayer,Player *otherPlayer, Resources resourceIn , Resources resourceOut , size_t inAmount , size_t outAmount)**: Trades resources between players.
- **static void decreaseResourcesAfterAction(Player* player,const std::string& action)**: Decreases resources after a specific action.
- **static bool hasEnoughResources(const Player* player ,const std::string& action)**: Checks if the player has enough resources for a specific action.
- **static void decreaseHalfOfAllResource(Player* player)**: Decreases half of all resources from the player.

### `RoadAndSettlementManagement`
Manages the placement of settlements and roads on the board.
- **static void placeSettlement(Player *player, size_t x, size_t y)**: Places a settlement on the board for the given player at the specified coordinates.
- **static void placeRoad(Player *player, size_t x1, size_t y1, size_t x2, size_t y2)**: Places a road on the board for the given player between the specified coordinates.
- **static void placeFirstSettlement(Player *player, size_t x, size_t y)**: Places the first settlement for the given player at the specified coordinates.
- **static void placeFirstRoad(Player *player, size_t x1, size_t y1, size_t x2, size_t y2)**: Places the first road for the given player between the specified coordinates.
- **static void allocateResourcesForSettlement(Player *player, Vertex *vertex)**: Allocates resources to the player for the newly placed settlement at the specified vertex.
- **static void upgradeToCity(Player *player, size_t x, size_t y)**: Upgrades a settlement to a city for the given player at the specified coordinates.

## Usage Instructions
- **make runtests**: Runs the tests for the game.
- **make runmain**: Runs the demo of the game.
- **make valgrind**: Checks for memory leaks using Valgrind.
