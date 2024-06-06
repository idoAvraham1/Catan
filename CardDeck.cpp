#include "CardDeck.hpp"

using namespace mycatan;

// Define the static member variables
std::vector<Card*> CardDeck::cards;
size_t CardDeck::biggestArmyCardsInDeck = 0; // Initialize to a default value
bool CardDeck::isInitialized = false; // Initialize to a default value

CardDeck::CardDeck() {
    initializeDeck();
    shuffleDeck();
}

CardDeck::~CardDeck() {
    cleanUp();
}

void CardDeck::initializeDeck() {
    isInitialized = true; // mark the deck as already initiated
    biggestArmyCardsInDeck = 3;
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

    if(isInitialized == false) // initiate the deck only once
        initializeDeck();

    if (cards.empty() && isInitialized ) {
        throw std::runtime_error("Cannot draw a card. The deck is empty.");
    }

    Card* drawnCard = cards.back();
    cards.pop_back();
    return drawnCard;
}

size_t CardDeck::getDeckSize() {
    return cards.size();
}

BiggestArmyCard* CardDeck::getBiggestArmyCard() {

    if(isInitialized == false) // initiate the deck only once
        initializeDeck();
    // verify that biggest army cards left in the deck 
    if(biggestArmyCardsInDeck == 0)
        throw std::runtime_error("No more biggest army cards in the deck!");

    // create a new biggest army card and return to player 
    auto* biggestArmyCard = new BiggestArmyCard();
    return biggestArmyCard;
}
void CardDeck::cleanUp(){
    for (Card* card : cards) {
        delete card;
    }
    cards.clear();
}