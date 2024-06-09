#ifndef CATAN_BOARD_HPP
#define CATAN_BOARD_HPP

#include "Catan_API.hpp"
#include "Tile.hpp"
#include "Player.hpp"


namespace mycatan {
    class Tile;
    class Edge;
    class Vertex;

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

        /**
         * @brief Public method to get the instance of the Board.
         * @return Pointer to the singleton Board instance.
         */
        static Board* getInstance();

        /**
         * @brief Destructor to clean up the Board.
         */
        ~Board();

        /**
         * @brief Clean up the Board by deleting all tiles, vertices, and edges.
         */
        void cleanBoard();

        /**
         * @brief Initialize the board with predefined tile configurations.
         */
        void initializeBoard();

        /**
         * @brief Allocate resources to players based on the dice roll.
         * @param diceRoll The result of the dice roll.
         */
        void allocateResources(size_t diceRoll);

        /**
         * @brief Get the tile based on ID and resource type.
         * @param id The ID of the tile.
         * @param resource The resource type of the tile.
         * @return Pointer to the Tile.
         */
        Tile* getTile(size_t id, Resources resource);

        /**
         * @brief Get the vertex based on coordinates.
         * @param x The x-coordinate of the vertex.
         * @param y The y-coordinate of the vertex.
         * @return Pointer to the Vertex.
         */
        Vertex* getVertex(size_t x, size_t y);

        /**
         * @brief Get the edge based on two vertices.
         * @param v1 Pointer to the first Vertex.
         * @param v2 Pointer to the second Vertex.
         * @return Pointer to the Edge.
         */
        Edge* getEdge(Vertex* v1, Vertex* v2);

        /**
         * @brief Check if a settlement can be placed on a vertex by a player.
         * @param player Pointer to the Player.
         * @param vertex Pointer to the Vertex.
         * @return True if the settlement can be placed, false otherwise.
         */
        bool canPlaceSettlement(Player* player, Vertex* vertex);

        /**
         * @brief Check if a road can be placed between two vertices by a player.
         * @param player Pointer to the Player.
         * @param vertex1 Pointer to the first Vertex.
         * @param vertex2 Pointer to the second Vertex.
         * @return True if the road can be placed, false otherwise.
         */
        bool canPlaceRoad(Player* player, Vertex* vertex1, Vertex* vertex2);

        /**
         * @brief Get the count of vertices on the board.
         * @return The count of vertices.
         */
        [[nodiscard]] size_t getVertexCount() const;

        /**
         * @brief Get the count of edges on the board.
         * @return The count of edges.
         */
        [[nodiscard]] size_t getEdgeCount() const;

        /**
         * @brief Get the tiles adjacent to a vertex.
         * @param vertex Pointer to the Vertex.
         * @return Vector of pointers to the adjacent Tiles.
         */
        std::vector<Tile *> getAdjacentTiles(Vertex *vertex) const;

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
