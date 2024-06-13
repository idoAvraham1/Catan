// written by Ido Avraham : 208699181
// EMAIL: idoavraham086@gmail.com

#ifndef CATAN_ROAD_AND_SETTLEMENT_MANAGEMENT_HPP
#define CATAN_ROAD_AND_SETTLEMENT_MANAGEMENT_HPP

#include "Board.hpp"
#include "Catan_API.hpp"
#include "Player.hpp"

namespace mycatan {
    class Vertex;

    class RoadAndSettlementManagement {
    public:
        // Place a settlement on the board for the given player at the specified coordinates
        static void placeSettlement(Player *player, size_t x, size_t y);

        // Place a road on the board for the given player between the specified coordinates
        static void placeRoad(Player *player, size_t x1, size_t y1, size_t x2, size_t y2);

        // Place the first settlement for the given player at the specified coordinates
        static void placeFirstSettlement(Player *player, size_t x, size_t y);

        // Place the first road for the given player between the specified coordinates
        static void placeFirstRoad(Player *player, size_t x1, size_t y1, size_t x2, size_t y2);

        // Allocate resources to the player for the newly placed settlement at the specified vertex
        static void allocateResourcesForSettlement(Player *player, Vertex *vertex);

        // Upgrade a settlement to a city for the given player at the specified coordinates
        static void upgradeToCity(Player *player, size_t x, size_t y);

    private:
        // Check if the given vertex coordinates are valid on the board
        static bool isValidVertexCoordinates(size_t x, size_t y);

        // Check if the given road coordinates are valid on the board
        static bool isValidRoadCoordinates(size_t x1, size_t y1, size_t x2, size_t y2);
    };
} // namespace mycatan

#endif // CATAN_ROAD_AND_SETTLEMENT_MANAGEMENT_HPP
