
#ifndef CATAN_RESOURCEMANAGEMENT_HPP
#define CATAN_RESOURCEMANAGEMENT_HPP
#include "Catan_API.hpp"
#include "Player.hpp"

namespace mycatan {
    class ResourceManagement {
    public:
        explicit ResourceManagement(Player* player);
        void addResource(Resources resource , size_t amount);
        void decreaseResource(Resources resource, size_t amount);
        size_t giveAllResourcesOfType(Resources resource);
        void tradeResources(Player *other, Resources resourceIn , Resources resourceOut , size_t inAmount , size_t outAmount);
        void decreaseResourcesAfterAction(const std::string& action);
        [[nodiscard]] bool hasEnoughResources(const std::string& action) const;
        void decreaseHalfOfAllResource();

    private:
        Player* player;

    };

}


#endif //CATAN_RESOURCEMANAGEMENT_HPP
