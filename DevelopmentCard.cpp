#include "DevelopmentCard.hpp"
#include "Player.hpp"

namespace mycatan {

// Base card implementation
    Card::Card(std::string type) : type(std::move(type)), owner(nullptr), used(false) {}

    std::string Card::getOwner() const {
        if (owner == nullptr)
            return "Card is still in the Deck (no owner)";
        return owner->getName();
    }

    bool Card::isUsed() const {
        return used;
    }

    void Card::setOwner(Player* player) {
        if (owner != nullptr)
            throw std::runtime_error("Card already has an owner!");
        owner = player;
    }

    void Card::setUsed() {
        used = true;
    }

// KnightCard Implementation
    KnightCard::KnightCard() : Card("Knight") {}
    std::string KnightCard::getType() const { return type; }

// BiggestArmyCard Implementation
    BiggestArmyCard::BiggestArmyCard() : Card("Biggest Army") {}
    std::string BiggestArmyCard::getType() const { return type; }

// MonopolyCard Implementation
    MonopolyCard::MonopolyCard() : Card("Monopoly") {}
    std::string MonopolyCard::getType() const { return type; }

// YearOfPlentyCard Implementation
    YearOfPlentyCard::YearOfPlentyCard() : Card("Year of Plenty") {}
    std::string YearOfPlentyCard::getType() const { return type; }

// WinningPointsCard Implementation
    WinningPointsCard::WinningPointsCard() : Card("Winning Points") {}
    std::string WinningPointsCard::getType() const { return type; }

// RoadCard Implemention
    RoadCard::RoadCard() : Card("Road") {}
    std::string RoadCard::getType() const {return type;}

} // namespace mycatan
