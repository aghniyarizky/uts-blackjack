// ═══════════════════════════════════════════════════════
//  SECTION 1 — KARTU (Struct + Linked List)
// ═══════════════════════════════════════════════════════

#ifndef LINNKED_CARD
#define LINNKED_CARD

#include "utility.h"


struct Card {
    string suit;    // Hearts, Diamonds, Clubs, Spades
    string rank;    // 2,3,4,5,6,7,8,9,10,J,Q,K,A
    int value;   // Nilai kartu = 2-10, 10 untuk JQK, 11 atau 1 untuk A (tergantung kondisi)
};

struct HandNode {
    Card card;       
    HandNode* next;       
    
    HandNode(Card c) : card(c), next(nullptr) {}
};

struct Hand {
    HandNode* head;      
    int size;    

    Hand() : head(nullptr), size(0) {}

    void addCard(Card c) {
        HandNode* node = new HandNode(c);
        if (!head) {
            head = node;
            size++;
            return;
        }
        HandNode* cur = head;
        while (cur->next) {
            cur = cur->next;
        }
        cur->next = node;
        size++;
    }
    int total() const {
        int sum = 0, aces = 0;
        HandNode* cur = head;
        while (cur) {
            sum += cur->card.value;
            if (cur->card.rank == "A") aces++;
            cur = cur->next;
        }

        // Jika total > 21 dan ada As, nilai As berubah dari 11 menjadi 1
        while (sum > 21 && aces > 0) {
            sum -= 10;
            aces--;
        }
        return sum;
    }

    ~Hand() { clear(); }

    void clear() {
        HandNode* cur = head;
        while (cur) {
            HandNode* t = cur;
            cur = cur->next;
            delete t;
        }
        head = nullptr;
        size = 0;
    }

    void print(bool hideFirst = false) const {
        HandNode* cur = head;
        int i = 0;
        while (cur) {
            if (hideFirst && i == 0)
                cout << " [???]";
            else
                cout << " [" << cur->card.rank << " " << cur->card.suit[0] << "]";
            cur = cur->next;
            i++;
        }
        cout << "\n";
    }
};

#endif
