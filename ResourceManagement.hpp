#ifndef CATAN_RESOURCEMANAGEMENT_HPP
#define CATAN_RESOURCEMANAGEMENT_HPP

#include "Catan_API.hpp"
#include "Player.hpp"

namespace mycatan {
    class ResourceManagement {
    public:

        static void addResource(Player* player,Resources resource , size_t amount);
        static void decreaseResource(Player* player , Resources resource, size_t amount);
        static size_t giveAllResourcesOfType(Player* player , Resources resource);
        static void tradeResources(Player* thisPlayer,Player *otherPlayer, Resources resourceIn , Resources resourceOut , size_t inAmount , size_t outAmount);
        static void decreaseResourcesAfterAction(Player* player,const std::string& action);
        static bool hasEnoughResources(const Player* player ,const std::string& action) ;
        static void decreaseHalfOfAllResource(Player* player);

    };
}

#endif //CATAN_RESOURCEMANAGEMENT_HPP
