#ifndef CATAN_VERTEX_HPP
#define CATAN_VERTEX_HPP

#include <string>
#include <vector>

#include "Player.hpp"
#include "Tile.hpp"

namespace mycatan {
class Player;

class Vertex {
   public:
    Vertex(size_t x, size_t y);

    [[nodiscard]] size_t getX() const;
    [[nodiscard]] size_t getY() const;

    [[nodiscard]] bool hasSettlement() const;
    [[nodiscard]] bool isCity() const;  // Optional, if you have cities
    [[nodiscard]] Player* getOwner() const;
    void setOwner(Player* newOwner);
    void buildSettlement(Player* newOwner);
    void upgradeToCity();

   private:
    size_t x, y;
    Player* owner;    // The player who owns the settlement/city
    bool settlement;  // True if a settlement is built here
    bool city;        // True if a city is built here
};

}  // namespace mycatan

#endif  // CATAN_VERTEX_HPP
