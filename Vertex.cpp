#include "Vertex.hpp"
using namespace mycatan;

Vertex::Vertex(size_t x, size_t y):x(x) , y(y),owner("None") {}


void Vertex::placeSettlement(const std::string& ownerName) {
    owner=ownerName;
}

size_t Vertex::getX() const {
    return x;
}

size_t Vertex::getY() const {
    return y;
}

bool Vertex::hasSettlement() const {
    if(owner == "None")
        return false;
    return true;
}
