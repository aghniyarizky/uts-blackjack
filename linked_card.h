// ═══════════════════════════════════════════════════════
//  SECTION 1 — KARTU & TANGAN (Struct + Linked List)
//  TUGAS ANGGOTA 3: Linked List khusus Game / Memori Tangan Berjalan
// ═══════════════════════════════════════════════════════

#ifndef LINNKED_CARD
#define LINNKED_CARD

#include "utility.h"

struct Card {
    string suit;
    string rank;
    int    value;
};

struct HandNode {
    Card      card;       // Data utama kartu
    HandNode* next;       // Pointer menuju node berikutnya
    
    // Constructor untuk memudahkan inisialisasi HandNode
    HandNode(Card c) : card(c), next(nullptr) {}
};

struct Hand {
    HandNode* head;       // Pointer ke kartu pertama (kepala/head)
    int       size;       // Jumlah kartu di tangan

    // Constructor: awalnya tangan kosong (digunakan saat pertama dibuat)
    Hand() : head(nullptr), size(0) {}

    // Fungsi menambah kartu. Node diletakkan di akhir list (Tail/Akhir)
    void addCard(Card c) {
        HandNode* node = new HandNode(c); // Buat simpul dinamis baru memori
        if (!head) { 
            // Jika tangan masih kosong, node menjadi kartu pertama
            head = node; 
            size++;
            return; 
        }
        
        // Melakukan traversal (penelusuran) dari node pertama sampai ujung belakang
        HandNode* cur = head;
        while (cur->next) {
            cur = cur->next;
        }
        // Menyambungkan node di akhir linked list
        cur->next = node;
        size++;
    }

    // Fungsi menghitung total nilai kartu dalam aturan Blackjack
    int total() const {
        int sum = 0, aces = 0;
        HandNode* cur = head;
        // Telusuri linked list dari head sampai ujung (nullptr)
        while (cur) {
            sum += cur->card.value;
            if (cur->card.rank == "A") aces++; // Hitung jumlah As
            cur = cur->next;
        }
        // Aturan khusus Blackjack: Jika total melebihi 21 dan ada kartu As, nilai As diubah dari 11 ke 1
        while (sum > 21 && aces > 0) { 
            sum -= 10; 
            aces--; 
        }
        return sum;
    }

    ~Hand() { clear(); }

    // Fungsi menghapus dan mendealokasikan seluruh node
    void clear() {
        HandNode* cur = head;
        while (cur) { 
            HandNode* t = cur; 
            cur = cur->next; 
            delete t; // Free memory! Mencegah memori aplikasi menumpuk.
        }
        head = nullptr; 
        size = 0;
    }

    // Fungsi mencetak kartu (opsi parameter bool untuk kartu dealer yang ditutup satu)
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