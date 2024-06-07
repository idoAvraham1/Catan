// just for commit
#include "Edge.hpp"
using namespace mycatan;

Edge::Edge(Vertex* v1, Vertex* v2): v1(v1) , v2(v2) , owner(nullptr) {}

Vertex* Edge::getVertex1() const {
    return v1;
}
Vertex* Edge::getVertex2() const {
    return v2;
}

bool Edge::hasRoad()const  {
    if(owner == nullptr)
        return false;
    return true;
}

void Edge::placeRoad( Player* newOwner) {
    owner = newOwner;
}
