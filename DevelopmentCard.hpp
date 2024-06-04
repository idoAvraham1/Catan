#ifndef CATAN_DEVELOPMENTCARD_HPP
#define CATAN_DEVELOPMENTCARD_HPP

#include "Catan_API.hpp"


namespace mycatan {

    class Player; // Forward declaration

    class Card {
    public:
        // Constructor
        explicit Card(std::string type);
        virtual ~Card() = default;

        // Pure virtual functions
        [[nodiscard]] virtual std::string getType() const = 0;

        // Common methods
        void setOwner(Player* player);
        [[nodiscard]] std::string getOwner() const;
        [[nodiscard]] bool isUsed() const;
        void setUsed();

    protected:
        // Attributes
        std::string type;
        Player* owner;
        bool used;
    };

    class KnightCard : public Card {
    public:
        KnightCard();
        [[nodiscard]] std::string getType() const override;
    };
    class RoadCard : public Card {
    public:
        RoadCard();
        [[nodiscard]] std::string getType() const override;
    };
    class BiggestArmyCard : public Card {
    public:
        BiggestArmyCard();
        [[nodiscard]] std::string getType() const override;
    };

    class MonopolyCard : public Card {
    public:
        MonopolyCard();
        [[nodiscard]] std::string getType() const override;
    };

    class YearOfPlentyCard : public Card {
    public:
        YearOfPlentyCard();
        [[nodiscard]] std::string getType() const override;
    };

    class WinningPointsCard : public Card {
    public:
        WinningPointsCard();
        [[nodiscard]] std::string getType() const override;
    };

} // namespace mycatan

#endif // CATAN_DEVELOPMENTCARD_HPP
