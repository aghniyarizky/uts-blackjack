// ═══════════════════════════════════════════════════════
//  SECTION 2 — PLAYER REGISTRY (Linked list CRUD)
//  TUGAS ANGGOTA 4: Single Linked List Profil Global & Operasi CRUD
// ═══════════════════════════════════════════════════════

#ifndef PLAYER_MNG
#define PLAYER_MNG

#include "utility.h"


struct PlayerProfile {
    int            id;
    string         name;
    int            chips;
    int            wins;
    int            losses;
    PlayerProfile* next;   // Pointer ke profil/node berikutnya di list

    // Constructor profil
    PlayerProfile(int i, string n, int c)
        : id(i), name(n), chips(c), wins(0), losses(0), next(nullptr) {}
};

// ── Registry: Pengelola Linked List PlayerProfile (List Utama) ──
struct PlayerRegistry {
    PlayerProfile* head;
    int            nextId;

    // Constructor
    PlayerRegistry() : head(nullptr), nextId(1) {}

    // ── CREATE: Membuat/Menambah Pemain Baru ──────────────────────────
    void addPlayer(string name, int startChips = 100, bool silent = false) {
        PlayerProfile* node = new PlayerProfile(nextId++, name, startChips);
        
        // Pengecekan Linked List kosong
        if (!head) { 
            head = node; 
        } else {
            // Traversal ke node akhir, lalu sambung pointer next-nya
            PlayerProfile* cur = head;
            while (cur->next) cur = cur->next;
            cur->next = node;
        }
        if (!silent)
            cout << "\n  [+] Pemain \"" << name << "\" ditambahkan (ID: " << node->id << ").\n";
    }

    // ── READ: Mencari Berdasarkan ID ──────────────────────────────────
    PlayerProfile* findById(int id) {
        PlayerProfile* cur = head;
        // Telusuri node satu persatu (Sequential Search Linked List)
        while (cur) {
            if (cur->id == id) return cur; // Jika cocok, kembalikan pointer (alamat memori object)
            cur = cur->next;
        }
        return nullptr; // Jika ID tidak ditemukan, kembalikan data hampa
    }

    // ── READ: Menampilkan Semua Pemain Leaderboard ────────────────────
    void showAll() const {
        if (!head) { cout << "\n  Tidak ada pemain yang terdaftar.\n"; return; }

        cout << "\n";
        cout << "  +---------+----------------------+--------+--------+--------+\n";
        cout << "  | ID      | Nama                 | Chips  | menang | kalah  |\n";
        cout << "  +---------+----------------------+--------+--------+--------+\n";

        PlayerProfile* cur = head;
        // Print iteratif menggunakan perulangan while menelusuri pointer next
        while (cur) {
            cout << "  | "
                 << left  << setw(7)  << cur->id    << " | "
                 << left  << setw(20) << cur->name  << " | "
                 << right << setw(6)  << cur->chips << " | "
                 << right << setw(6)  << cur->wins  << " | "
                 << right << setw(6)  << cur->losses << " |\n";
            cur = cur->next;
        }
        cout << "  +---------+----------------------+--------+--------+--------+\n";
    }

    void updatePlayer(int id) {
        PlayerProfile* p = findById(id);
        if (!p) {
            cout << "\n  +----------------------------------------------------+\n";
            cout << "  |       [GAGAL] ID " << id << " tidak ditemukan.";
            // pad agar | kanan sejajar (W=51, prefix="  | " sudah di atas)
            string msg = "       [GAGAL] ID " + to_string(id) + " tidak ditemukan.";
            int pad = 52 - (int)msg.size(); if (pad < 0) pad = 0;
            for (int i = 0; i < pad; i++) cout << " ";
            cout << "|\n";
            cout << "  +----------------------------------------------------+\n\n";
            return;
        }

        auto bRow = [](const string& t) {
            const int W = 51;
            cout << "  | " << t;
            int pad = W - (int)t.size(); if (pad < 0) pad = 0;
            for (int i = 0; i < pad; i++) cout << " ";
            cout << "|\n";
        };
        auto bCenter = [](const string& t) {
            const int W = 51;
            // Word-wrap jika terlalu panjang
            string rem = t;
            while (!rem.empty()) {
                int cut = (int)rem.size() <= W ? (int)rem.size() : W;
                if ((int)rem.size() > W) {
                    while (cut > 0 && rem[cut] != ' ') cut--;
                    if (cut == 0) cut = W;
                }
                string chunk = rem.substr(0, cut);
                int lp = (W - (int)chunk.size()) / 2;
                int rp = W - (int)chunk.size() - lp;
                if (lp < 0) lp = 0; if (rp < 0) rp = 0;
                cout << "  | ";
                for (int i = 0; i < lp; i++) cout << " ";
                cout << chunk;
                for (int i = 0; i < rp; i++) cout << " ";
                cout << "|\n";
                rem = rem.substr(cut);
                while (!rem.empty() && rem[0] == ' ') rem = rem.substr(1);
            }
        };
        auto bBorder     = []() { cout << "  +----------------------------------------------------+\n"; };
        auto bBorderThin = []() { cout << "  |----------------------------------------------------|\n"; };

        int choice = 0;
        do {
            cout << "\n";
            bBorder();
            bCenter("EDIT PEMAIN: " + p->name);
            bCenter("Chips saat ini: " + to_string(p->chips));
            bBorderThin();
            bRow("  [ 1 ]  Ganti nama");
            bRow("  [ 2 ]  Ganti chips");
            bRow("  [ 3 ]  Ganti nama & chips");
            bBorder();
            cout << "  Pilih opsi: ";
            choice = getIntInput();
            if (choice < 1 || choice > 3) {
                bBorder();
                bCenter("[GAGAL] Pilihan tidak valid! Masukkan angka 1, 2, atau 3.");
                bBorder();
                cout << "\n";
            }
        } while (choice < 1 || choice > 3);

        if (choice == 1 || choice == 3) {
            cout << "  Nama baru (maks 15 karakter): ";
            p->name = getNameInput();
        }
        if (choice == 2 || choice == 3) {
            cout << "  Jumlah chips baru (1-999999): ";
            p->chips = getChipsInput();
        }
        bBorder();
        bCenter("[ OK ] Data " + p->name + " berhasil diperbarui.");
        bBorder();
        cout << "\n";
    }

    // ── DELETE: Menghapus Profil Dari Linked List ─────────────────────
    void deletePlayer(int id) {
        if (!head) { cout << "\n  Tidak ada pemain yang bisa dihapus.\n"; return; }

        // Kondisi 1: Jika yang mau dihapus adalah head (node paling pertama)
        if (head->id == id) {
            PlayerProfile* temp = head; // Simpan sementara pointer lama
            head = head->next;          // Jadikan node kedua sebagai head baru
            cout << "\n  [-] Pemain \"" << temp->name << "\" dihapus.\n";
            delete temp;                // Hapus free() ruang memorinya
            return;
        }

        // Kondisi 2: Node tersembunyi di tengah jalan atau akhir
        PlayerProfile* prev = head;
        // Cari node TEPAT SEBELUM node target (mencari prev->next)
        while (prev->next && prev->next->id != id)
            prev = prev->next;

        // Jika sampai ujung tapi data tidak cocok
        if (!prev->next) { cout << "\n  ID pemain " << id << " tidak ditemukan.\n"; return; }

        // Proses Bypassing / Memutus dan menyambungkan rel Jalur List Baru
        PlayerProfile* target = prev->next; // Inilah titik yang dituju
        prev->next = target->next;          // Sambung pointer sebelum target langsung melewati target ke data selanjutnya
        cout << "\n  [-] Pemain \"" << target->name << "\" dihapus.\n";
        delete target; // Memori dikembalikan ke sistem!
    }

    // Destructor (Pembersih Ram otomatis saat program selesai / Object Out-of-Scope)
    ~PlayerRegistry() { clear(); }

    // Membebaskan secara iteratif
    void clear() {
        PlayerProfile* cur = head;
        while (cur) { 
            PlayerProfile* t = cur; 
            cur = cur->next; 
            delete t; 
        }
        head = nullptr;
    }
};

#endif