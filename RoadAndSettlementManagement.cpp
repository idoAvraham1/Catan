#include "RoadAndSettlementManagement.hpp"
#include "ResourceManagement.hpp"

namespace mycatan {
    void RoadAndSettlementManagement::placeSettlement(Player* player, size_t x, size_t y) {
        Board* board = Board::getInstance();
        Vertex* vertex = board->getVertex(x, y);
        if (canPlaceSettlement(player, vertex) && ResourceManagement::hasEnoughResources(player, "settlement")) {
            vertex->buildSettlement(player);
            player->settlements.push_back(vertex);
            ResourceManagement::decreaseResourcesAfterAction(player, "settlement");
            std::cout << player->getName() << " placed a settlement at (" << x << ", " << y << ")" << std::endl;
        } else {
            std::cout << "Cannot place settlement at (" << x << ", " << y << ")" << std::endl;
        }
    }

    void RoadAndSettlementManagement::placeRoad(Player* player, size_t x1, size_t y1, size_t x2, size_t y2) {
        Board* board = Board::getInstance();
        Vertex* vertex1 = board->getVertex(x1, y1);
        Vertex* vertex2 = board->getVertex(x2, y2);
        Edge* edge = board->getEdge(vertex1, vertex2);

        if (canPlaceRoad(player, vertex1, vertex2) && ResourceManagement::hasEnoughResources(player, "road")) {
            edge->placeRoad(player);
            player->roads.push_back(edge);
            ResourceManagement::decreaseResourcesAfterAction(player, "road");
            std::cout << player->getName() << " placed a road between (" << x1 << ", " << y1 << ") and (" << x2 << ", " << y2 << ")" << std::endl;
        } else {
            std::cout << "Cannot place road between (" << x1 << ", " << y1 << ") and (" << x2 << ", " << y2 << ")" << std::endl;
        }
    }

    bool RoadAndSettlementManagement::canPlaceSettlement(Player* player, Vertex* vertex) {
        if (vertex == nullptr || vertex->hasSettlement()) {
            return false; // The vertex is already occupied
        }
        // Additional checks if needed
        return true;
    }

    bool RoadAndSettlementManagement::canPlaceRoad(Player* player, Vertex* vertex1, Vertex* vertex2) {
        Board* board = Board::getInstance();
        Edge* edge = board->getEdge(vertex1, vertex2);
        if (edge == nullptr || edge->hasRoad()) {
            return false; // The edge already has a road
        }
        // Additional checks if needed
        return true;
    }
}
