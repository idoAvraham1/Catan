// written by Ido Avraham : 208699181
// EMAIL: idoavraham086@gmail.com
#include "Board.hpp"

using namespace mycatan;

// Initialize static member
Board *Board::boardInstance = nullptr;

// Define the predefined tile configuration for the default Catan board
std::map<std::pair<size_t, size_t>, TileConfig> Board::tileConfigurations = {
        // first row
        {{0, 4}, {Resources::Brick, 5}},
        {{1, 4}, {Resources::Wheat, 6}},
        {{2, 4}, {Resources::Wool, 11}},
        // second row
        {{0, 3}, {Resources::Wood, 8}},
        {{1, 3}, {Resources::Ore, 3}},
        {{2, 3}, {Resources::Wheat, 4}},
        {{3, 3}, {Resources::Wool, 7}},
        // third row
        {{0, 2}, {Resources::Wheat, 9}},
        {{1, 2}, {Resources::Wood, 11}},
        {{2, 2}, {Resources::Desert, 7}},
        {{3, 2}, {Resources::Wood, 3}},
        {{4, 2}, {Resources::Ore, 8}},
        // fourth row
        {{1, 1}, {Resources::Wheat, 12}},
        {{2, 1}, {Resources::Brick, 6}},
        {{3, 1}, {Resources::Wool, 4}},
        {{4, 1}, {Resources::Brick, 10}},
        // fifth row
        {{2, 0}, {Resources::Ore, 10}},
        {{3, 0}, {Resources::Wool, 2}},
        {{4, 0}, {Resources::Wood, 9}},
};

// Private constructor
Board::Board() {
    initializeBoard();
}

void Board::cleanup() {
    if (boardInstance != nullptr) {
        delete boardInstance;
        boardInstance = nullptr;
    }
}

void Board::cleanBoard() {
    // Delete all tiles
    for (Tile* tile : tiles) {
        delete tile;
    }
    tiles.clear();

    // Delete all vertices
    for (auto& vertexPair : vertices) {
        delete vertexPair.second;
    }
    vertices.clear();

    // Delete all edges
    for (auto& edgePair : edges) {
        delete edgePair.second;
    }
    edges.clear();
}

Board::~Board() {
    std::cout<<"Cleaning the board"<<std::endl;
    cleanBoard();
}

// Public method to get the singleton instance
Board *Board::getInstance() {
    if (boardInstance == nullptr) {
        boardInstance = new Board();
    }
    return boardInstance;
}

void Board::initializeBoard() {
    std::cout << "Setting up the board" << std::endl;
    for (const auto &[coordinates, token] : tileConfigurations) {
        size_t x = coordinates.first;
        size_t y = coordinates.second;
        Resources resource = token.resourceType;
        size_t id = token.numberToken;
        createTile(x, y, id, resource);
    }
}

Vertex *Board::getOrCreateVertex(size_t x, size_t y) {
    auto key = std::make_pair(x, y);
    // If the vertex doesn't exist, create it
    if (vertices.find(key) == vertices.end()) {
        vertices[key] = new Vertex(x, y);
    }
    return vertices[key];
}

Edge *Board::getOrCreateEdge(Vertex *v1, Vertex *v2) {
    auto key = std::make_pair(std::min(v1, v2), std::max(v1, v2));
    // If the edge doesn't exist, create it
    if (edges.find(key) == edges.end()) {
        edges[key] = new Edge(v1, v2);
    }
    return edges[key];
}

void Board::createTile(size_t x, size_t y, size_t id, Resources resourceType) {
    // Calculate the top-left vertex for the hexagon
    size_t u = 2 * (x - 1) + y;
    size_t v = y;
    // Calculate vertices for the hexagon
    Vertex *v1 = getOrCreateVertex(u, v);          // Top-Left Vertex
    Vertex *v2 = getOrCreateVertex(u + 1, v);      // Top Vertex
    Vertex *v3 = getOrCreateVertex(u + 2, v);      // Top-Right Vertex
    Vertex *v4 = getOrCreateVertex(u + 2, v + 1);  // Bottom-Right Vertex
    Vertex *v5 = getOrCreateVertex(u + 1, v + 1);  // Bottom Vertex
    Vertex *v6 = getOrCreateVertex(u, v + 1);      // Bottom-Left Vertex

    // Link vertices to the tile
    std::vector<Vertex *> hexVertices = {v1, v2, v3, v4, v5, v6};
    std::vector<Edge *> hexEdges;

    // Create edges for the hexagon
    hexEdges.push_back(getOrCreateEdge(v1, v2));
    hexEdges.push_back(getOrCreateEdge(v2, v3));
    hexEdges.push_back(getOrCreateEdge(v3, v4));
    hexEdges.push_back(getOrCreateEdge(v4, v5));
    hexEdges.push_back(getOrCreateEdge(v5, v6));
    hexEdges.push_back(getOrCreateEdge(v6, v1));

    // Create the hexagon tile
    Tile *tile = new Tile(id, resourceType, hexVertices, hexEdges);

    // Associate vertices with the tile
    for (Vertex *vertex : hexVertices) {
        vertexToTiles[vertex].push_back(tile);
    }

    tiles.push_back(tile);
}

Tile *Board::getTile(size_t id, Resources resource) {
    for (Tile *tile : tiles) {
        if (tile->getId() == id && tile->getResourceType() == resource) {
            return tile;
        }
    }
    return nullptr;
}

std::vector<Tile *> Board::getAdjacentTiles(Vertex *vertex) const {
    std::vector<Tile *> adjacentTiles;

    // Get the tiles adjacent to the given vertex
    if (vertexToTiles.find(vertex) != vertexToTiles.end()) {
        adjacentTiles = vertexToTiles.at(vertex);
    }

    return adjacentTiles;
}

Vertex *Board::getVertex(size_t x, size_t y) {
    auto key = std::make_pair(x, y);
    // Return the vertex if it exists
    if (vertices.find(key) != vertices.end()) {
        return vertices[key];
    }
    return nullptr;
}

Edge *Board::getEdge(Vertex *v1, Vertex *v2) {
    auto key = std::make_pair(std::min(v1, v2), std::max(v1, v2));
    // Return the edge if it exists
    if (edges.find(key) != edges.end()) {
        return edges[key];
    }
    return nullptr;
}

bool Board::canPlaceSettlement(Player *player, Vertex *vertex) {
    if (vertex == nullptr || vertex->hasSettlement()) {
        return false;  // The vertex is already occupied
    }

    // Check if the player has a road leading to this vertex
    for (Edge *road : player->getRoads()) {
        if (road->getVertex1() == vertex || road->getVertex2() == vertex) {
            return true;  // The player has a road leading to this vertex
        }
    }

    return false;  // No road leads to this vertex
}

bool Board::canPlaceRoad(Player *player, Vertex *vertex1, Vertex *vertex2) {
    // Check that the road has no other owner
    Edge *road = getEdge(vertex1, vertex2);
    if (road->hasRoad()) {
        return false;
    }

    // Check that the player has a settlement or road connected to this road
    for (Vertex *settlement : player->getSettlements()) {
        if (road->isContainVertex(vertex1) || road->isContainVertex(vertex2)) {
            return true;
        }
    }

    for (Edge *existingRoad : player->getRoads()) {
        if (existingRoad->getVertex1() == vertex1 || existingRoad->getVertex2() == vertex1 ||
            existingRoad->getVertex1() == vertex2 || existingRoad->getVertex2() == vertex2) {
            return true;
        }
    }

    return false;
}

void Board::allocateResources(size_t diceRoll) {
    for (Tile *tile : tiles) {
        if (tile->getId() == diceRoll) {
            // The tile's number token matches the dice roll
            for (Vertex *vertex : tile->getVertices()) {
                if (vertex->hasSettlement()) {
                    Player *player = vertex->getOwner();
                    size_t amount = vertex->isCity() ? 2 : 1;  // cities produce double resources
                    player->addResource(tile->getResourceType(), amount);

                    // Print the resource allocation details
                    std::cout << player->getName() << " received " << amount << " unit(s) of "
                              << resourceToString(tile->getResourceType()) << " from the settlement at ("
                              << vertex->getX() << ", " << vertex->getY() << ")" << std::endl;
                }
            }
        }
    }
}

size_t Board::getVertexCount() const {
    return vertices.size();
}

size_t Board::getEdgeCount() const {
    return edges.size();
}

