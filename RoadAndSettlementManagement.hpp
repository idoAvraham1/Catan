#ifndef ROAD_AND_SETTLEMENT_MANAGEMENT_HPP
#define ROAD_AND_SETTLEMENT_MANAGEMENT_HPP

#include "Player.hpp"
#include "Board.hpp"

namespace mycatan {
    class RoadAndSettlementManagement {
    public:
        static void placeSettlement(Player* player, size_t x, size_t y);
        static void placeRoad(Player* player, size_t x1, size_t y1, size_t x2, size_t y2);
        static bool canPlaceSettlement(Player* player, Vertex* vertex);
        static bool canPlaceRoad(Player* player, Vertex* vertex1, Vertex* vertex2);
    };
}

#endif // ROAD_AND_SETTLEMENT_MANAGEMENT_HPP
