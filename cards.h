// ═══════════════════════════════════════════════════════
//  SECTION 3 — DECK (Array 1D + Manipulation Array Pointing)
// ═══════════════════════════════════════════════════════

#ifndef CARDS
#define CARDS

#include "utility.h"

const int DECK_SIZE = 52;
Card deck[DECK_SIZE];   // jumlah kartu = 52
int  deckIndex = 0;     

void buildDeck() {
    string suits[]  = {"Hearts","Diamonds","Clubs","Spades"};
    string ranks[]  = {"2","3","4","5","6","7","8","9","10","J","Q","K","A"};
    int    values[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};
    int i = 0;
    
    for (int s = 0; s < 4; s++) {
        for (int r = 0; r < 13; r++) {
            deck[i] = {suits[s], ranks[r], values[r]};
            i++;
        }
    }
}

void shuffleDeck() {
    for (int i = DECK_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card tmp = deck[i];
        deck[i] = deck[j];
        deck[j] = tmp;
    }
    deckIndex = 0;
}

Card* dealCard() {
    if (deckIndex >= DECK_SIZE) { 
        shuffleDeck(); 
        cout << "  [Deck diacak ulang]\n"; 
    }
    return &deck[deckIndex++];
}

#endif