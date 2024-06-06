#ifndef CATAN_CATAN_API_HPP
#define CATAN_CATAN_API_HPP

// includes
#include <map>
#include <string>
#include <vector>
#include <unordered_map>
#include <random>
#include <iostream>
#include <utility>
#include <stdexcept>
#include <algorithm>

// defines
#define BRICK 0
#define WHEAT 1
#define WOOL  2
#define WOOD  3
#define ORE 4

#define NEXT_PLAYER 0
#define BEFORE_PLAYER 1

// enums
enum class Resources { Brick , Wheat , Wool , Wood , Ore, Desert };
enum class Lands { Forest, Hills, Pasture, Fields, Mountains, Desert };

inline std::string resourceToString(Resources resource) {
    switch (resource) {
        case Resources::Brick: return "Brick";
        case Resources::Wheat: return "Wheat";
        case Resources::Wool: return "Wool";
        case Resources::Wood: return "Wood";
        case Resources::Ore : return "Ore";
        case Resources::Desert: return "Desert";
        default: return "Unknown";
    }
}

inline size_t resourceToInt(Resources resource){
    switch(resource) {
        case Resources::Brick: return 0;
        case Resources::Wheat: return 1;
        case Resources::Wool: return 2;
        case Resources::Wood: return 3;
        case Resources::Ore: return 4;
        default: throw std::runtime_error("Invalid Resource!");
    }
}


#endif // CATAN_CATAN_API_HPP
