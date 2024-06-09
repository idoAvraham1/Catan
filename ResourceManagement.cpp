#include "ResourceManagement.hpp"
#include "Player.hpp"

namespace mycatan {

    void ResourceManagement::addResource(Player *player, Resources resource, size_t amount) {
        if (resource == Resources::Desert)
            return; // avoid adding resources if the resource is desert

        size_t resourceIndex = resourceToInt(resource);
        player->resources[resourceIndex] += amount;
    }

    void ResourceManagement::decreaseResource(Player *player, Resources resource, size_t amount) {
        size_t resourceIndex = resourceToInt(resource);
        if (player->resources[resourceIndex] < amount) {
            throw std::runtime_error("Not enough resources!");
        }
        player->resources[resourceIndex] -= amount;
    }

    void ResourceManagement::decreaseHalfOfAllResource(Player *player) {
        for (size_t &resource: player->resources) {
            resource = resource / 2; // Integer division to halve the resources
        }
    }

    size_t ResourceManagement::giveAllResourcesOfType(Player *player, Resources resource) {
        size_t amount = player->resources[resourceToInt(resource)];
        player->resources[resourceToInt(resource)] = 0;
        return amount;
    }

    void ResourceManagement::tradeResources(Player *thisPlayer, Player *otherPlayer, Resources resourceIn,
                                            Resources resourceOut, size_t inAmount, size_t outAmount) {
        if (!thisPlayer->isMyTurn) {
            throw std::runtime_error("It's not your turn!");
        }

        size_t resourceInIndex = resourceToInt(resourceIn);
        size_t resourceOutIndex = resourceToInt(resourceOut);

        if (thisPlayer->resources[resourceOutIndex] < outAmount || otherPlayer->resources[resourceInIndex] < inAmount) {
            throw std::runtime_error("Don't have enough resources to trade!");
        }

        decreaseResource(thisPlayer, resourceOut, outAmount);
        decreaseResource(otherPlayer, resourceIn, inAmount);
        addResource(thisPlayer, resourceIn, inAmount);
        addResource(otherPlayer, resourceOut, outAmount);

        std::cout << thisPlayer->name << " traded " << outAmount << " units of " << resourceToString(resourceOut)
                  << " with " << otherPlayer->name << " for " << inAmount << " units of "
                  << resourceToString(resourceIn) << std::endl;
    }

    void ResourceManagement::decreaseResourcesAfterAction(Player *player, const std::string &action) {
        static std::unordered_map<std::string, std::vector<size_t>> resourceRequirements = {
                {"settlement", {1, 1, 1, 1, 0}},       // 1 Brick, 1 Wheat, 1 Wool, 1 Wood
                {"road", {1, 0, 1, 0, 0}},             // 1 Brick, 1 Wood
                {"city", {0, 2, 3, 0, 0}},             // 2 Wheat, 3 Ore
                {"developmentCard", {0, 1, 1, 0, 1}}   // 1 Wheat, 1 Wool, 1 Ore
        };

        const auto& requirements = resourceRequirements[action];
        player->resources[BRICK] -= requirements[BRICK];
        player->resources[WHEAT] -= requirements[WHEAT];
        player->resources[WOOL] -= requirements[WOOL];
        player->resources[WOOD] -= requirements[WOOD];
        player->resources[ORE] -= requirements[ORE];
    }

    bool ResourceManagement::hasEnoughResources(const Player *player, const std::string &action) {
        static std::unordered_map<std::string, std::vector<size_t>> resourceRequirements = {
                {"settlement",      {1, 1, 1, 1, 0}},       // 1 Brick, 1 Wheat, 1 Wool, 1 Wood
                {"road",            {1, 0, 1, 0, 0}},             // 1 Brick, 1 Wood
                {"city",            {0, 2, 3, 0, 0}},             // 2 Wheat, 3 Ore
                {"developmentCard", {0, 1, 1, 0, 1}}   // 1 Wheat, 1 Wool, 1 Ore
        };

        const auto &requirements = resourceRequirements[action];
        return player->resources[BRICK] >= requirements[BRICK] &&
               player->resources[WHEAT] >= requirements[WHEAT] &&
               player->resources[WOOL] >= requirements[WOOL] &&
               player->resources[WOOD] >= requirements[WOOD] &&
               player->resources[ORE] >= requirements[ORE];

    }
}