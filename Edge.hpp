// written by Ido Avraham : 208699181
// EMAIL: idoavraham086@gmail.com

#ifndef CATAN_EDGE_HPP
#define CATAN_EDGE_HPP

#include "Catan_API.hpp"
#include "Player.hpp"
#include "Vertex.hpp"

namespace mycatan {
class Vertex;
class Edge {
   public:
    Edge(Vertex* v1, Vertex* v2);
    [[nodiscard]] Vertex* getVertex1() const;
    [[nodiscard]] Vertex* getVertex2() const;
    [[nodiscard]] bool hasRoad() const;
    Player* getOwner() const;
    void placeRoad(Player* newOwner);
    bool isContainVertex(Vertex* vertex);

   private:
    Vertex* v1;
    Vertex* v2;
    Player* owner;
};
}  // namespace mycatan

#endif  // CATAN_EDGE_HPP
