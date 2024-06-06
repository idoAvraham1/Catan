#ifndef CATAN_BOARD_HPP
#define CATAN_BOARD_HPP

#include "Catan_API.hpp"
#include "Tile.hpp"
#include "Player.hpp"


namespace mycatan {

    // Structure to hold resource type and number token
    struct TileConfig {
        Resources resourceType;
        size_t numberToken;
    };

    // Singleton class representing the board
    class Board {
    public:
        // Delete copy constructor and assignment operator to enforce singleton pattern
        Board(const Board& obj) = delete;
        Board& operator=(const Board& obj) = delete;

        // Public method to get the instance of the Board
        static Board* getInstance();

        // Destructor
        ~Board();

        // Methods for board initialization and resource allocation
        void initializeBoard();
        void allocateResources(size_t diceRoll);

        // Methods to interact with the board
        Tile* getTile(size_t id, Resources resource);
        Vertex* getVertex(size_t x, size_t y);
        Edge* getEdge(Vertex* v1, Vertex* v2);
        bool canPlaceSettlement(Player* player, Vertex* vertex);
        bool canPlaceRoad(Player* player ,Vertex* vertex1 , Vertex* vertex2 );

        // Methods to get the count of vertices and edges
        [[nodiscard]] size_t getVertexCount() const;
        [[nodiscard]] size_t getEdgeCount() const;

        // Static member to hold the predefined tile configuration
        static std::map<std::pair<size_t, size_t>, TileConfig> tileConfigurations;

    private:
        // Private constructor for the singleton pattern
        Board();

        // Singleton instance
        static Board* boardInstance;

        // Data members to store tiles, vertices, edges, and vertex-to-tiles association
        std::vector<Tile*> tiles;
        std::map<std::pair<size_t, size_t>, Vertex*> vertices; // Map to store unique vertices
        std::map<std::pair<Vertex*, Vertex*>, Edge*> edges; // Map to store unique edges
        std::map<Vertex*, std::vector<Tile*>> vertexToTiles; // Map to store vertex-to-tiles association

        // Helper methods to create or get vertices and edges
        Vertex* getOrCreateVertex(size_t x, size_t y);
        Edge* getOrCreateEdge(Vertex* v1, Vertex* v2);
        void createTile(size_t x, size_t y, size_t id, Resources resourceType);
    };

} // namespace mycatan

#endif // CATAN_BOARD_HPP
