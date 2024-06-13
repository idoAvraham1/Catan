// written by Ido Avraham : 208699181
// EMAIL: idoavraham086@gmail.com
#include "CardDeck.hpp"
using namespace mycatan;



// Define the static member variables
std::vector<Card*> CardDeck::cards;
CardDeck* instance = nullptr;
size_t CardDeck::biggestArmyCardsInDeck = 0;  // Initialize to a default value


// get instance of the deck
CardDeck* CardDeck::getInstance(){
    if(instance == nullptr)
        instance = new CardDeck();
    return instance;
}

// Constructor: initializes and shuffles the deck
CardDeck::CardDeck() {
    initializeDeck();
    shuffleDeck();
}

// Destructor: cleans up the deck by deleting all dynamically allocated cards
CardDeck::~CardDeck() {
    std::cout<<"Cleaning the card deck"<<std::endl;
    cleanDeck();
}

// Clean up the deck by deleting all dynamically allocated cards
void CardDeck::cleanDeck() {
    for (Card* card : cards) {
        delete card;
    }
    cards.clear();
}

// clean the deck ptr
void CardDeck::cleanup() {
    // Clean up the singleton instance
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

// Initialize the deck with predefined cards
void CardDeck::initializeDeck() {
    biggestArmyCardsInDeck = 3;
    cards.clear();  // Clear any existing cards before initializing

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

// Shuffle the deck using a random generator
void CardDeck::shuffleDeck() {
    std::random_device rd;                        // Obtain a random number from hardware
    std::mt19937 g(rd());                         // Seed the generator
    std::shuffle(cards.begin(), cards.end(), g);  // Shuffle the cards
    std::cout << "Deck shuffled." << std::endl;
}

// Draw a card from the deck
Card* CardDeck::drawCard() {
    if (instance == nullptr)  // Initialize the deck only once
         instance = new CardDeck();

    if (cards.empty() && instance != nullptr) {  // Check if the deck is empty
        throw std::runtime_error("Cannot draw a card. The deck is empty.");
    }

    // Get the last card in the deck, remove the card from deck and return it
    Card* drawnCard = cards.back();
    cards.pop_back();
    return drawnCard;
}

// Get the current size of the deck
size_t CardDeck::getDeckSize() {
    return cards.size();
}

// Get the Biggest Army card from the deck
BiggestArmyCard* CardDeck::getBiggestArmyCard() {
    if (instance == nullptr)  // Initialize the deck only once
        instance = new CardDeck();

    // verify that biggest army cards left in the deck
    if (biggestArmyCardsInDeck == 0)
        throw std::runtime_error("No more biggest army cards in the deck!");

    // create a new biggest army card and return to player
    auto* biggestArmyCard = new BiggestArmyCard();
    --biggestArmyCardsInDeck;  // Decrement the count of biggest army cards
    return biggestArmyCard;
}

// get the biggest army card back from a player (e.g after a trade with other player of knight card)
void CardDeck::returnBiggestArmyCard(Card* biggestArmyCard) {
    biggestArmyCardsInDeck++;
    delete biggestArmyCard;
}


