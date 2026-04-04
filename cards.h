
// ═══════════════════════════════════════════════════════
//  SECTION 3 — DECK (Array 1D + Manipulation Array Pointing)
//  TUGAS ANGGOTA 2: Manajemen Array Konstan dan Randomisasi Angka
// ═══════════════════════════════════════════════════════

#ifndef CARDS
#define CARDS

#include "utility.h"

const int DECK_SIZE = 52;
Card deck[DECK_SIZE];   // ARRAY Statis yang dibiarkan global untuk kemudahan akses
int  deckIndex = 0;     // Penanda kartu ke-berapa yang akan dibagikan

// Menginisialisasi isi array deck 52 kartu layaknya deck pada dunia nyata
void buildDeck() {
    string suits[]  = {"Hearts","Diamonds","Clubs","Spades"};
    string ranks[]  = {"2","3","4","5","6","7","8","9","10","J","Q","K","A"};
    int    values[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};
    int i = 0;
    
    // Nested loop menggabungkan 4 jenis bunga (suit) ke 13 varian nilai (rank).
    for (int s = 0; s < 4; s++) {
        for (int r = 0; r < 13; r++) {
            deck[i] = {suits[s], ranks[r], values[r]};
            i++;
        }
    }
}

// Randomizer sederhana 
void shuffleDeck() {
    for (int i = DECK_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1); // cari indeks acak
        // Lakukan teknik penukaran Swap untuk mengacak (mengikuti pola iteratif dari ujung ke depan)
        Card tmp = deck[i]; 
        deck[i] = deck[j]; 
        deck[j] = tmp;
    }
    deckIndex = 0; // Kembalikan counter tumpukan paling atas ke permulaan list
}

Card* dealCard() {
    // Apabila tumpukan semua sudah terambil, kembalikan tumpukan dan acak kembali
    if (deckIndex >= DECK_SIZE) { shuffleDeck(); cout << "  [Deck diacak ulang]\n"; }
    
    // Mengembalikan elemen array sekarang (&deck) lalu post-increment / tambahkan nilai deckIndex-nya
    return &deck[deckIndex++];
}

#endif