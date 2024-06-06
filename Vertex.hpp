#ifndef CATAN_VERTEX_HPP
#define CATAN_VERTEX_HPP
#include "Catan_API.hpp"

namespace mycatan {
    class Vertex {
    public:
        Vertex(size_t x, size_t y);
        void placeSettlement(const std::string& ownerName);
        [[nodiscard]] bool hasSettlement() const ;
        [[nodiscard]] size_t getX() const;
        [[nodiscard]] size_t getY() const;


    private:
        std::string owner;
        size_t x;
        size_t y;
    };
}

#endif //CATAN_VERTEX_HPP
