#include "RoadAndSettlementManagement.hpp"

#include "ResourceManagement.hpp"

namespace mycatan {

void RoadAndSettlementManagement::placeSettlement(Player *player, size_t x, size_t y) {
    if (!player->isMyTurn)
        std::__throw_runtime_error("Not your turn!");

    Board *board = Board::getInstance();
    Vertex *vertex = board->getVertex(x, y);
    if (board->canPlaceSettlement(player, vertex) && ResourceManagement::hasEnoughResources(player, "settlement")) {
        vertex->buildSettlement(player);
        player->settlements.push_back(vertex);
        ResourceManagement::decreaseResourcesAfterAction(player, "settlement");
        player->winning_points++;
        allocateResourcesForSettlement(player, vertex);
        std::cout << player->getName() << " placed a settlement at (" << x << ", " << y << ")" << std::endl;
    } else {
        std::cout << "Cannot place settlement at (" << x << ", " << y << ")" << std::endl;
    }
}

void RoadAndSettlementManagement::placeRoad(Player *player, size_t x1, size_t y1, size_t x2, size_t y2) {
    if (!player->isMyTurn)
        std::__throw_runtime_error("Not your turn!");

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
        std::cout << "Cannot place road between (" << x1 << ", " << y1 << ") and (" << x2 << ", " << y2 << ")"
                  << std::endl;
    }
}

void RoadAndSettlementManagement::placeFirstSettlement(Player *player, size_t x, size_t y) {
    // first time trying to place the first settlement
    if (player->placedFirstSettlements || !player->isMyTurn)
        throw std::runtime_error("Already placed you first settlements!");

    Board *board = Board::getInstance();
    Vertex *vertex = board->getVertex(x, y);
    if (vertex && !vertex->hasSettlement()) {
        vertex->buildSettlement(player);
        player->settlements.push_back(vertex);
        player->winning_points++;
        allocateResourcesForSettlement(player, vertex);
        std::cout << player->getName() << " placed the first settlement at (" << x << ", " << y << ")" << std::endl;
    } else {
        std::cout << "Cannot place the first settlement at (" << x << ", " << y << ")" << std::endl;
    }
}

void RoadAndSettlementManagement::placeFirstRoad(Player *player, size_t x1, size_t y1, size_t x2, size_t y2) {
    if (!player->isMyTurn)
        std::__throw_runtime_error("Not your turn!");

    if (player->placedFirstRoads)  // first time trying to place the first roads
        throw std::runtime_error("Already placed your first Roads!");

    Board *board = Board::getInstance();
    Vertex *vertex1 = board->getVertex(x1, y1);
    Vertex *vertex2 = board->getVertex(x2, y2);
    Edge *edge = board->getEdge(vertex1, vertex2);

    if (vertex1 && vertex2 && edge && !edge->hasRoad()) {
        edge->placeRoad(player);
        player->roads.push_back(edge);
        std::cout << player->getName() << " placed a road between (" << x1 << ", " << y1 << ") and (" << x2 << ", "
                  << y2 << ")" << std::endl;
    } else {
        std::cout << "Cannot place road between (" << x1 << ", " << y1 << ") and (" << x2 << ", " << y2 << ")"
                  << std::endl;
    }
}

void RoadAndSettlementManagement::allocateResourcesForSettlement(Player *player, Vertex *vertex) {
    Board *board = Board::getInstance();
    std::vector<Tile *> adjacentTiles = board->getAdjacentTiles(vertex);
    for (Tile *tile : adjacentTiles) {
        ResourceManagement::addResource(player, tile->getResourceType(), 1);
        std::cout << player->getName() << " collected 1 unit of " << resourceToString(tile->getResourceType())
                  << std::endl;
    }
}

void RoadAndSettlementManagement::upgradeToCity(Player *player, size_t x, size_t y) {
    if (!player->isMyTurn)
        std::__throw_runtime_error("Not your turn!");

    // Verify that the player has enough resources and a settlement in the given vertex

    Board *board = Board::getInstance();
    Vertex *toUpgrade = board->getVertex(x, y);

    if (toUpgrade && toUpgrade->getOwner() == player && !toUpgrade->isCity() &&
        ResourceManagement::hasEnoughResources(player, "city")) {
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
