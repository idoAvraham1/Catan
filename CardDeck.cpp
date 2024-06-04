#include "CardDeck.hpp"

using namespace mycatan;

// Initialize the static member
std::vector<Card*> CardDeck::cards;

CardDeck::CardDeck() {
    initializeDeck();
    shuffleDeck();
}

CardDeck::~CardDeck() {
    cleanUp();
}

CardDeck& CardDeck::getInstance() {
    static CardDeck instance; // Guaranteed to be destroyed, instantiated on first use
    return instance;
}

void CardDeck::initializeDeck() {
    if (!cards.empty()) return;  // Avoid reinitializing if already done

    // Add Knight cards
    for (int i = 0; i < 14; ++i) {
        cards.push_back(new KnightCard());
    }

    // Add Monopoly cards
    for (int i = 0; i < 2; ++i) {
        cards.push_back(new MonopolyCard());
    }

    // Add Year of Plenty cards
    for (int i = 0; i < 2; ++i) {
        cards.push_back(new YearOfPlentyCard());
    }

    // Add Road Building cards
    for (int i = 0; i < 2; ++i) {
        cards.push_back(new RoadCard());
    }

    // Add Victory Point cards
    for (int i = 0; i < 5; ++i) {
        cards.push_back(new WinningPointsCard());
    }

    std::cout << "Deck initialized with " << cards.size() << " cards." << std::endl;
}

void CardDeck::shuffleDeck() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
    std::cout << "Deck shuffled." << std::endl;
}

Card* CardDeck::drawCard() {
    if (cards.empty()) {
        throw std::runtime_error("Cannot draw a card. The deck is empty.");
    }

    Card* drawnCard = cards.back();
    cards.pop_back();
    return drawnCard;
}

size_t CardDeck::getDeckSize() {
    return cards.size();
}

BiggestArmyCard* CardDeck::getBiggestArmyCard(){
    // verify that biggest army cards left in the deck 
    if(BiggestArmyCardsInDeck == 0)
        throw std::runtime_error("No more biggest army cards in the deck!");

    // create a new biggest army card and return to player 
    BiggestArmyCard* biggestArmyCard = new BiggestArmyCard();
    return biggestArmyCard;
}
void CardDeck::cleanUp(){
    for (Card* card : cards) {
        delete card;
    }
    cards.clear();
}