// written by Ido Avraham : 208699181
// EMAIL: idoavraham086@gmail.com

#include "Tile.hpp"
using namespace mycatan;

Tile::Tile(size_t id, Resources resourceType ,const std::vector<Vertex*>& vertices, const std::vector<Edge*>& edges)
        : id(id), resourceType(resourceType) ,vertices(vertices), edges(edges) {}

size_t Tile::getId() const {
    return id;
}

std::vector<Vertex*> Tile::getVertices() const {
    return vertices;
}

std::vector<Edge*> Tile::getEdges() const {
    return edges;
}

Resources Tile::getResourceType() {
    return resourceType;
}
