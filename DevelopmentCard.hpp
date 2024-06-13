// written by Ido Avraham : 208699181
// EMAIL: idoavraham086@gmail.com

#ifndef CATAN_DEVELOPMENTCARD_HPP
#define CATAN_DEVELOPMENTCARD_HPP

#include "Catan_API.hpp"


namespace mycatan {

    class Player; // Forward declaration

    class Card {
    public:
        /**
          * @brief Constructor for the Card class.
          * @param type Type of the card.
          */
        explicit Card(std::string type);
        virtual ~Card() = default;

        /**
         * @brief Get the type of the card.
         * @return The type of the card as a string.
         */
        [[nodiscard]] virtual std::string getType() const = 0;

        /**
         * @brief Set the owner of the card.
         * @param player Pointer to the player who owns the card.
         */
        void setOwner(Player* player);

        /**
        * @brief Get the name of the card owner.
        * @return Name of the owner or a message if the card is still in the deck.
        */
        [[nodiscard]] std::string getOwner() const;

        /**
        * @brief Check if the card has been used.
        * @return True if the card is used, false otherwise.
        */
        [[nodiscard]] bool isUsed() const;

        /**
         * @brief Mark the card as used.
         */
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
