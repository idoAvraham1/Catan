#include "ResourceManagement.hpp"
#include "Player.hpp"
namespace mycatan {

    ResourceManagement::ResourceManagement(mycatan::Player *player) : player(player) {}

    void ResourceManagement::addResource(Resources resource, size_t amount) {
        size_t resourceIndex = resourceToInt(resource);
        player->resources[resourceIndex] += amount;
    }

    void ResourceManagement::decreaseResource(Resources resource, size_t amount) {
        size_t resourceIndex = resourceToInt(resource);
        if (player->resources[resourceIndex] < amount) {
            throw std::runtime_error("Not enough resources!");
        }
        player->resources[resourceIndex] -= amount;
    }

    void ResourceManagement::decreaseHalfOfAllResource(){
        for (size_t& resource : player->resources) {
            resource = resource / 2; // Integer division to halve the resources
        }
    }
    size_t ResourceManagement::giveAllResourcesOfType(Resources resource) {
        size_t amount = player->resources[resourceToInt(resource)];
        player->resources[resourceToInt(resource)] = 0;
        return amount;
    }

    void ResourceManagement::tradeResources(Player *other, Resources resourceIn, Resources resourceOut, size_t inAmount, size_t outAmount) {
        if(!this->player->isMyTurn)
            throw std::runtime_error("Its not your turn!");

        // get the resources to trade index in the resource vector
        size_t resourceIn_index = resourceToInt(resourceIn);
        size_t resourceOut_Index = resourceToInt(resourceOut);

        // check each player has enough resources
        if (this->player->resources[resourceOut_Index] < outAmount || other->resources[resourceIn_index] < inAmount)
            throw std::runtime_error("Dont have enough resources to trade!");

        // perform the trade - add resourceIn amount to this, decrease resourceOut to other, and vice versa
        this->decreaseResource(resourceOut, outAmount);
        other->resourceManager->decreaseResource(resourceIn, inAmount);
        this->addResource(resourceIn, inAmount);
        other->resourceManager->addResource(resourceOut, outAmount);

        std::cout << this->player->name << " Traded " << std::to_string((int) outAmount) << " units of "
                  << resourceToString(resourceOut)
                  << " with: " << other->name << " for " << std::to_string((int) resourceIn) << " units of: "
                  << resourceToString(resourceIn) << std::endl;
    }

    void ResourceManagement::decreaseResourcesAfterAction(const std::string &action) {
        static std::unordered_map<std::string, std::vector<size_t>> resourceRequirements = {
                {"settlement",      {1, 1, 1, 1}},
                {"road",            {1, 1, 0, 0}},
                {"city",            {0, 2, 3, 0}},
                {"developmentCard", {0, 1, 1, 1}}
        };

        const auto &requirements = resourceRequirements[action];
        player->resources[WOOD] -= requirements[WOOD];
        player->resources[BRICK] -= requirements[BRICK];
        player->resources[WHEAT] -= requirements[WHEAT];
        player->resources[WOOL] -= requirements[WOOL];
    }

    bool ResourceManagement::hasEnoughResources(const std::string &action) const {
        static std::unordered_map<std::string, std::vector<size_t>> resourceRequirements = {
                {"settlement",      {1, 1, 1, 1}},
                {"road",            {1, 1, 0, 0}},
                {"city",            {0, 2, 3, 0}},
                {"developmentCard", {0, 1, 1, 1}}
        };

        const auto &requirements = resourceRequirements[action];
        return player->resources[WOOD] >= requirements[WOOD] &&
               player->resources[BRICK] >= requirements[BRICK] &&
               player->resources[WHEAT] >= requirements[WHEAT] &&
               player->resources[WOOL] >= requirements[WOOL];
    }


}