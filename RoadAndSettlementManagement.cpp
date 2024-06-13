// written by Ido Avraham : 208699181
// EMAIL: idoavraham086@gmail.com

#include "RoadAndSettlementManagement.hpp"
#include "ResourceManagement.hpp"

namespace mycatan {

// Check if vertex coordinates are valid
bool RoadAndSettlementManagement::isValidVertexCoordinates(size_t x , size_t y){
    Board *board = Board::getInstance();
    Vertex *vertex = board->getVertex(x, y);
    return vertex != nullptr;
}
// Check if road coordinates are valid
bool RoadAndSettlementManagement::isValidRoadCoordinates(size_t x1 , size_t y1 , size_t x2 , size_t y2){
    Board *board = Board::getInstance();
    Vertex *vertex1 = board->getVertex(x1, y1);
    Vertex* vertex2  = board->getVertex(x2 , y2);
    Edge* edge = board->getEdge(vertex1 , vertex2);
    return edge != nullptr;
}

void RoadAndSettlementManagement::placeSettlement(Player *player, size_t x, size_t y) {
    if (!player->isMyTurn) // verify the current turn
        std::__throw_runtime_error("Not your turn!");

    if (!isValidVertexCoordinates(x, y)) { // verify valid coordinate
        throw std::runtime_error("Invalid coordinates for placing a settlement: (" + std::to_string(x) + ", " + std::to_string(y) + ")");
    }

    Board *board = Board::getInstance();
    Vertex *vertex = board->getVertex(x, y);

    // if player has enough resources and can build a settlement, build the settlement and add to his settlements
    if (board->canPlaceSettlement(player, vertex) && ResourceManagement::hasEnoughResources(player, "settlement")) {
        vertex->buildSettlement(player);
        player->settlements.push_back(vertex);
        ResourceManagement::decreaseResourcesAfterAction(player, "settlement");

        player->winning_points++; // gain 1 wining point from the new settlement built
        std::cout << player->getName() << " placed a settlement at (" << x << ", " << y << ")" << std::endl;
    } else {
        throw std::runtime_error("Cannot place settlement at (" + std::to_string(x) + ", " + std::to_string(y) + ")");
    }
}

void RoadAndSettlementManagement::placeRoad(Player *player, size_t x1, size_t y1, size_t x2, size_t y2) {
    if (!player->isMyTurn)
        std::__throw_runtime_error("Not your turn!");

    if (!isValidRoadCoordinates(x1, y1, x2, y2)) {
        throw std::runtime_error("Invalid coordinates for placing a road between (" + std::to_string(x1) + ", " + std::to_string(y1) + ") and (" + std::to_string(x2) + ", " + std::to_string(y2) + ")");
    }

    Board *board = Board::getInstance();
    Vertex *vertex1 = board->getVertex(x1, y1);
    Vertex *vertex2 = board->getVertex(x2, y2);
    Edge *edge = board->getEdge(vertex1, vertex2);

    // verify that the player has enough resources for building road, and that the
    // desired road is valid
    if (board->canPlaceRoad(player, vertex1, vertex2) && ResourceManagement::hasEnoughResources(player, "road")) {
        edge->placeRoad(player);
        player->roads.push_back(edge);
        ResourceManagement::decreaseResourcesAfterAction(player, "road");
        std::cout << player->getName() << " placed a road between (" << x1 << ", " << y1 << ") and (" << x2 << ", "
                  << y2 << ")" << std::endl;
    } else {  // road placement isn't valid
        throw std::runtime_error("Cannot place road between (" + std::to_string(x1) + ", " + std::to_string(y1) + ") and (" + std::to_string(x2) + ", " + std::to_string(y2) + ")");

    }
}

void RoadAndSettlementManagement::placeFirstSettlement(Player *player, size_t x, size_t y) {
    if (player->placedFirstSettlements || !player->isMyTurn) {
        throw std::runtime_error("Already placed your first settlements or not your turn!");
    }

    if (!isValidVertexCoordinates(x, y)) {
        throw std::runtime_error("Invalid coordinates for placing a settlement: (" + std::to_string(x) + ", " + std::to_string(y) + ")");
    }

    Board *board = Board::getInstance();
    Vertex *vertex = board->getVertex(x, y);
    if (vertex && !vertex->hasSettlement()) {
        vertex->buildSettlement(player);
        player->settlements.push_back(vertex);
        player->winning_points++;
        allocateResourcesForSettlement(player, vertex);
        std::cout << player->getName() << " placed the first settlement at (" << x << ", " << y << ")" << std::endl;
    } else {
        throw std::runtime_error("Cannot place settlement at (" + std::to_string(x) + ", " + std::to_string(y) + ")");
    }
}

void RoadAndSettlementManagement::placeFirstRoad(Player *player, size_t x1, size_t y1, size_t x2, size_t y2) {
    if (!player->isMyTurn) {
        std::__throw_runtime_error("Not your turn!");
    }

    if (player->placedFirstRoads) {
        throw std::runtime_error("Already placed your first roads!");
    }

    if (!isValidRoadCoordinates(x1, y1, x2, y2)) {
        throw std::runtime_error("Invalid coordinates for placing a road between (" + std::to_string(x1) + ", " + std::to_string(y1) + ") and (" + std::to_string(x2) + ", " + std::to_string(y2) + ")");
    }

    Board *board = Board::getInstance();
    Vertex *vertex1 = board->getVertex(x1, y1);
    Vertex *vertex2 = board->getVertex(x2, y2);
    Edge *edge = board->getEdge(vertex1, vertex2);

    // if the player can build a road, build the road and add him to his owned roads
    if (vertex1 && vertex2 && edge && !edge->hasRoad()) {
        edge->placeRoad(player);
        player->roads.push_back(edge);
        std::cout << player->getName() << " placed a road between (" << x1 << ", " << y1 << ") and (" << x2 << ", "
                  << y2 << ")" << std::endl;
    } else {
        throw std::runtime_error("Cannot place road between (" + std::to_string(x1) + ", " + std::to_string(y1) + ") and (" + std::to_string(x2) + ", " + std::to_string(y2) + ")");
    }
}

void RoadAndSettlementManagement::allocateResourcesForSettlement(Player *player, Vertex *vertex) {
    // get instance and the board and all the tiles connected to the player settlements
    Board *board = Board::getInstance();
    std::vector<Tile *> adjacentTiles = board->getAdjacentTiles(vertex);

    // run onto the tiles and add the according resource to the player
    for (Tile *tile : adjacentTiles) {
        ResourceManagement::addResource(player, tile->getResourceType(), 1);
        std::cout << player->getName() << " collected 1 unit of " << resourceToString(tile->getResourceType())
                  << std::endl;
    }
}

void RoadAndSettlementManagement::upgradeToCity(Player *player, size_t x, size_t y) {
    if (!player->isMyTurn)
        std::__throw_runtime_error("Not your turn!");

    if (!isValidVertexCoordinates(x, y)) {
        throw std::runtime_error("Invalid coordinates for upgrading to a city: (" + std::to_string(x) + ", " + std::to_string(y) + ")");
    }

    // Verify that the player has enough resources and a settlement in the given vertex,
    // and that the settlement isn't yet a city
    Board *board = Board::getInstance();
    Vertex *toUpgrade = board->getVertex(x, y);

    if (toUpgrade && toUpgrade->getOwner() == player && !toUpgrade->isCity() &&
        ResourceManagement::hasEnoughResources(player, "city") && !toUpgrade->isCity()) {
        std::cout << player->getName() << " upgraded a settlement to a city at (" << x << ", " << y << ")" << std::endl;
        // Upgrade the settlement to a city
        toUpgrade->upgradeToCity();
        player->winning_points++;  // Player gains one more point for the city

        // Decrease resources for upgrading to a city
        ResourceManagement::decreaseResourcesAfterAction(player, "city");
    } else {
        throw std::runtime_error("Cannot upgrade settlement to a city");
    }
}

}  // namespace mycatan
