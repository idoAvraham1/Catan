// written by Ido Avraham : 208699181
// EMAIL: idoavraham086@gmail.com

#ifndef CATAN_TILE_HPP
#define CATAN_TILE_HPP
#include "Catan_API.hpp"
#include "Edge.hpp"
#include "Vertex.hpp"

namespace mycatan {
class Vertex;
class Edge;
class Tile {
   public:
    Tile(size_t id, Resources resourceType, const std::vector<Vertex*>& vertices, const std::vector<Edge*>& edges);

    [[nodiscard]] size_t getId() const;
    [[nodiscard]] std::vector<Vertex*> getVertices() const;
    [[nodiscard]] std::vector<Edge*> getEdges() const;
    [[nodiscard]] Resources getResourceType();

   private:
    size_t id;
    Resources resourceType;
    std::vector<Vertex*> vertices;
    std::vector<Edge*> edges;
};
}  // namespace mycatan

#endif  // CATAN_TILE_HPP
