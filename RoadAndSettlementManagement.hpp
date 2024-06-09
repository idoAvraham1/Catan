#ifndef CATAN_ROAD_AND_SETTLEMENT_MANAGEMENT_HPP
#define CATAN_ROAD_AND_SETTLEMENT_MANAGEMENT_HPP

#include "Catan_API.hpp"
#include "Player.hpp"
#include "Board.hpp"


namespace mycatan {
    class Vertex;
    class RoadAndSettlementManagement {
    public:
        static void placeSettlement(Player* player, size_t x, size_t y);
        static void placeRoad(Player* player, size_t x1, size_t y1, size_t x2, size_t y2);
        static void placeFirstSettlement(Player* player, size_t x, size_t y);
        static void placeFirstRoad(Player* player, size_t x1, size_t y1, size_t x2, size_t y2);
        static void allocateResourcesForSettlement(Player* player, Vertex* vertex);
        static void upgradeToCity(Player* player ,size_t x , size_t y);

    };
} // namespace mycatan

#endif // CATAN_ROAD_AND_SETTLEMENT_MANAGEMENT_HPP
