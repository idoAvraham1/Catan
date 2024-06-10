#include "Vertex.hpp"

namespace mycatan {

Vertex::Vertex(size_t x, size_t y) : x(x), y(y), owner(nullptr), settlement(false), city(false) {
}

size_t Vertex::getX() const {
    return x;
}

size_t Vertex::getY() const {
    return y;
}

bool Vertex::hasSettlement() const {
    return settlement;
}

bool Vertex::isCity() const {
    return city;
}

Player* Vertex::getOwner() const {
    return owner;
}

void Vertex::setOwner(Player* newOwner) {
    this->owner = newOwner;
}

void Vertex::buildSettlement(Player* newOwner) {
    this->owner = newOwner;
    this->settlement = true;
}

void Vertex::upgradeToCity() {
    if (settlement) {
        city = true;
    }
}

}  // namespace mycatan
