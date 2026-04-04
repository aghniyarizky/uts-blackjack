/*
 * ============================================================
 *  BLACKJACK + PLAYER MANAGEMENT SYSTEM in C++
 *  (VERSI DENGAN PENJELASAN / KOMENTAR DETAIL)
 *
 *  Konsep C++ yang Digunakan:
 *    - struct       : Untuk membuat tipe data bentukan (Card, HandNode, dll).
 *    - array        : Untuk menyimpan 52 kartu (deck).
 *    - pointer      : Untuk merujuk pada alamat memori (dealCard(), linked list).
 *    - linked list  : Struktur data dinamis untuk Profil Pemain (CRUD) dan kartu di tangan (Hand).
 *    - destructor   : Untuk membersihkan memori secara otomatis saat objek dihapus.
 *    - file I/O     : Untuk menyimpan data pemain secara permanen ke file.
 *
 *  Operasi CRUD (Create, Read, Update, Delete):
 *    - CREATE  : Menambah profil pemain baru
 *    - READ    : Menampilkan semua pemain (leaderboard)
 *    - UPDATE  : Mengubah nama atau jumlah chips pemain
 *    - DELETE  : Menghapus profil pemain
 * ============================================================
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iomanip>
#include <fstream>      // Untuk file I/O (menyimpan data pemain)
#include <algorithm>    // Untuk transform (case insensitive)
#include <cctype>       // Untuk tolower
#include <limits>       // Untuk numeric_limits
using namespace std;

// ============================================================
// FUNGSI UTILITY DASAR
// ============================================================

/*
 * clearScreen()
 * Membersihkan layar terminal agar tampilan lebih rapi.
 * Bekerja di Windows (cls) dan Linux/Mac (clear).
 */
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/*
 * waitEnter() - DIPERBAIKI
 * Fungsi untuk menunggu user menekan ENTER satu kali saja.
 * 
 * MASALAH SEBELUMNYA: 
 * Setelah getIntInput() atau input lainnya, ada newline tersisa di buffer.
 * cin.get() membaca newline tersebut langsung, tanpa menunggu user.
 * 
 * SOLUSI: 
 * Gunakan cin.ignore() untuk membersihkan buffer terlebih dahulu,
 * baru kemudian cin.get() untuk menunggu ENTER dari user.
 */
void waitEnter() {
    // Bersihkan buffer input dari karakter tersisa (seperti newline)
    // cin.ignore(numeric_limits<streamsize>::max(), '\n');
    // Tunggu user menekan ENTER
    cin.get();
}

/*
 * toLowercase()
 * Mengubah string menjadi huruf kecil semua (case insensitive).
 * Contoh: "RaFaH" menjadi "rafah"
 */
string toLowercase(string str) {
    for (int i = 0; i < (int)str.length(); i++) {
        str[i] = tolower(str[i]);
    }
    return str;
}

/*
 * printInlineError()
 * Mencetak pesan error dalam box yang rapi.
 * Fungsi ini dipanggil ketika user melakukan input yang salah.
 */
void printInlineError(const string& msg) {
    const int W = 51;
    cout << "\n  +";
    for (int i = 0; i < W + 1; i++) cout << "-";
    cout << "+\n";

    string rem = "[GAGAL] " + msg;
    bool first = true;
    while (!rem.empty()) {
        int avail = first ? W : (W - 8);
        string pfx = first ? "" : "        ";
        int cut = (int)rem.size() <= avail ? (int)rem.size() : avail;
        if ((int)rem.size() > avail) {
            while (cut > 0 && rem[cut] != ' ') cut--;
            if (cut == 0) cut = avail;
        }
        string line = pfx + rem.substr(0, cut);
        cout << "  | " << line;
        for (int i = 0; i < W - (int)line.size(); i++) cout << " ";
        cout << "|\n";
        rem = rem.substr(cut);
        while (!rem.empty() && rem[0] == ' ') rem = rem.substr(1);
        first = false;
    }

    cout << "  +";
    for (int i = 0; i < W + 1; i++) cout << "-";
    cout << "+\n\n";
}

/*
 * getIntInput()
 * Meminta input angka dari user.
 * Jika user memasukkan huruf, akan looping sampai benar.
 */
int getIntInput() {
    int val;
    while (!(cin >> val)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        printInlineError("Input tidak valid. Masukkan angka yang benar.");
        cout << "  > ";
    }
    // Hapus newline setelah angka, tapi JANGAN di-ignore di sini
    // Biarkan waitEnter() yang menangani
    return val;
}

/*
 * getYesNoInput()
 * Meminta input y/n dari user.
 * Validasi hanya menerima y, Y, n, N.
 */
char getYesNoInput() {
    char c;
    while (true) {
        cin >> c;
        // Hapus newline setelah karakter
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (c == 'y' || c == 'Y' || c == 'n' || c == 'N') return c;
        printInlineError("Input tidak valid. Masukkan 'y' atau 'n'.");
        cout << "  > ";
    }
}

/*
 * getUsernameInput()
 * Meminta input username dengan batasan:
 *   - Maksimal 15 karakter
 *   - Tidak boleh kosong
 *   - Hanya satu kata (tanpa spasi)
 */
string getUsernameInput() {
    const int MAX_NAME = 15;
    string name;
    while (true) {
        cin >> name;
        // Hapus newline setelah username
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (name.empty()) {
            printInlineError("Username tidak boleh kosong.");
            cout << "  > ";
            continue;
        }
        if ((int)name.size() > MAX_NAME) {
            printInlineError("Username terlalu panjang! Maksimal " + to_string(MAX_NAME) +
                             " karakter (kamu memasukkan " + to_string((int)name.size()) + ").");
            cout << "  > ";
            continue;
        }
        break;
    }
    return name;
}

/*
 * getPasswordInput()
 * Meminta input password dengan syarat minimal 8 karakter.
 * Digunakan untuk password baru saat registrasi atau ganti password.
 */
string getPasswordInput() {
    const int MIN_PASS = 8;
    string pass;
    while (true) {
        getline(cin, pass);
        
        // Hapus carriage return (\r) jika ada (dari Windows)
        while (!pass.empty() && (pass.back() == '\r' || pass.back() == '\n')) {
            pass.pop_back();
        }
        
        if (pass.empty()) {
            printInlineError("Password tidak boleh kosong.");
            cout << "  > ";
            continue;
        }
        
        if ((int)pass.length() < MIN_PASS) {
            printInlineError("Password minimal " + to_string(MIN_PASS) + 
                           " karakter (kamu memasukkan " + to_string((int)pass.length()) + ").");
            cout << "  > ";
            continue;
        }
        
        break;
    }
    return pass;
}

/*
 * getConfirmPasswordInput() - FUNGSI BARU
 * Meminta input konfirmasi password TANPA validasi minimal karakter.
 * Hanya memastikan input tidak kosong.
 * Digunakan untuk konfirmasi password saat registrasi atau ganti password.
 */
string getConfirmPasswordInput() {
    string pass;
    while (true) {
        getline(cin, pass);
        
        // Hapus carriage return (\r) jika ada (dari Windows)
        while (!pass.empty() && (pass.back() == '\r' || pass.back() == '\n')) {
            pass.pop_back();
        }
        
        if (pass.empty()) {
            printInlineError("Konfirmasi password tidak boleh kosong.");
            cout << "  > ";
            continue;
        }
        
        break;
    }
    return pass;
}

/*
 * getChipsInput()
 * Meminta input jumlah chips dengan batasan 1-999999.
 */
int getChipsInput() {
    const int MAX_CHIPS = 999999;
    int val;
    while (true) {
        val = getIntInput();
        if (val < 1) {
            printInlineError("Chips minimal 1.");
            cout << "  > ";
            continue;
        }
        if (val > MAX_CHIPS) {
            printInlineError("Chips maksimal " + to_string(MAX_CHIPS) +
                             ". Kamu memasukkan " + to_string(val) + ".");
            cout << "  > ";
            continue;
        }
        break;
    }
    return val;
}

/*
 * getMenuInput()
 * Meminta input angka dalam range tertentu (minVal - maxVal).
 */
int getMenuInput(int minVal, int maxVal) {
    while (true) {
        int val = getIntInput();
        if (val >= minVal && val <= maxVal) return val;
        printInlineError("Pilihan tidak valid. Masukkan angka " +
                         to_string(minVal) + "-" + to_string(maxVal) + ".");
        cout << "  > ";
    }
}

// ============================================================
// SECTION 1 — KARTU & TANGAN (Struct + Linked List)
// TUGAS ANGGOTA 3: Linked List khusus Game / Memori Tangan Berjalan
// ============================================================

/*
 * Struct Card
 * Mewakili satu kartu remi dengan suit (hati/keriting/dll), 
 * rank (2-10,J,Q,K,A), dan nilai dalam blackjack.
 */
struct Card {
    string suit;    // Hearts, Diamonds, Clubs, Spades
    string rank;    // 2,3,4,5,6,7,8,9,10,J,Q,K,A
    int    value;   // Nilai dalam blackjack (2-10, 10 untuk JQK, 11 untuk A)
};

/*
 * Struct HandNode
 * Node untuk linked list tangan pemain.
 * Setiap node menyimpan satu kartu dan pointer ke node berikutnya.
 */
struct HandNode {
    Card      card;       // Data kartu
    HandNode* next;       // Pointer ke node berikutnya
    
    // Constructor: membuat node baru dengan kartu tertentu
    HandNode(Card c) : card(c), next(nullptr) {}
};

/*
 * Struct Hand
 * Linked list untuk menyimpan kartu di tangan pemain/dealer.
 * Memiliki fungsi untuk menambah kartu, menghitung total, dan membersihkan memori.
 */
struct Hand {
    HandNode* head;       // Pointer ke kartu pertama
    int       size;       // Jumlah kartu di tangan

    // Constructor: tangan kosong saat pertama dibuat
    Hand() : head(nullptr), size(0) {}

    /*
     * addCard()
     * Menambahkan kartu baru ke akhir linked list.
     */
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

    /*
     * total()
     * Menghitung total nilai kartu sesuai aturan blackjack.
     * As bisa bernilai 11 atau 1 (otomatis disesuaikan jika melebihi 21).
     */
    int total() const {
        int sum = 0, aces = 0;
        HandNode* cur = head;
        while (cur) {
            sum += cur->card.value;
            if (cur->card.rank == "A") aces++;
            cur = cur->next;
        }
        // Jika total > 21 dan ada As, ubah nilai As dari 11 menjadi 1
        while (sum > 21 && aces > 0) {
            sum -= 10;
            aces--;
        }
        return sum;
    }

    // Destructor: membersihkan memori otomatis
    ~Hand() { clear(); }

    /*
     * clear()
     * Menghapus semua node dalam linked list dan membebaskan memori.
     */
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

    /*
     * print()
     * Mencetak kartu-kartu dalam tangan.
     * Parameter hideFirst: untuk menyembunyikan kartu pertama dealer.
     */
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

// ============================================================
// SECTION 2 — PLAYER REGISTRY (Linked list CRUD + File I/O)
// TUGAS ANGGOTA 4: Single Linked List Profil Global & Operasi CRUD
// ============================================================

/*
 * Struct PlayerProfile
 * Profil pemain yang juga berfungsi sebagai Node dalam linked list.
 * Menyimpan username, password, chips, statistik menang/kalah.
 */
struct PlayerProfile {
    int            id;
    string         username;    // Nama pengguna (case insensitive untuk login)
    string         password;    // Password (case sensitive)
    int            chips;       // Jumlah chips saat ini
    int            wins;        // Jumlah kemenangan
    int            losses;      // Jumlah kekalahan
    PlayerProfile* next;        // Pointer ke node berikutnya

    // Constructor: membuat profil pemain baru
    PlayerProfile(int i, string u, string p, int c)
        : id(i), username(u), password(p), chips(c), wins(0), losses(0), next(nullptr) {}
};

/*
 * Struct PlayerRegistry
 * Pengelola linked list profil pemain dengan operasi CRUD.
 * Dilengkapi dengan penyimpanan ke file (permanen).
 */
struct PlayerRegistry {
    PlayerProfile* head;
    int            nextId;
    string         filename;    // Nama file untuk menyimpan data

    // Constructor: memuat data dari file saat program dimulai
    PlayerRegistry(const string& fname = "players.dat") 
        : head(nullptr), nextId(1), filename(fname) {
        loadFromFile();
    }

    /*
     * saveToFile()
     * Menyimpan semua data pemain ke file.
     * Format: id|username|password|chips|wins|losses
     * Dipanggil setiap kali ada perubahan data (tambah, edit, hapus, main).
     */
    void saveToFile() {
        ofstream file(filename);
        if (!file.is_open()) return;
        
        PlayerProfile* cur = head;
        while (cur) {
            file << cur->id << "|"
                 << cur->username << "|"
                 << cur->password << "|"
                 << cur->chips << "|"
                 << cur->wins << "|"
                 << cur->losses << "\n";
            cur = cur->next;
        }
        file.close();
    }

    /*
     * loadFromFile()
     * Memuat data pemain dari file saat program dimulai.
     * Jika file tidak ada, data akan kosong.
     */
    void loadFromFile() {
        ifstream file(filename);
        if (!file.is_open()) return;
        
        clear();  // Bersihkan data yang ada
        
        string line;
        int maxId = 0;
        
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            // Parsing format: id|username|password|chips|wins|losses
            size_t pos = 0;
            size_t nextPos = line.find('|', pos);
            int id = stoi(line.substr(pos, nextPos - pos));
            
            pos = nextPos + 1;
            nextPos = line.find('|', pos);
            string username = line.substr(pos, nextPos - pos);
            
            pos = nextPos + 1;
            nextPos = line.find('|', pos);
            string password = line.substr(pos, nextPos - pos);
            
            pos = nextPos + 1;
            nextPos = line.find('|', pos);
            int chips = stoi(line.substr(pos, nextPos - pos));
            
            pos = nextPos + 1;
            nextPos = line.find('|', pos);
            int wins = stoi(line.substr(pos, nextPos - pos));
            
            pos = nextPos + 1;
            int losses = stoi(line.substr(pos));
            
            // Buat node baru
            PlayerProfile* node = new PlayerProfile(id, username, password, chips);
            node->wins = wins;
            node->losses = losses;
            node->next = nullptr;
            
            // Tambahkan ke linked list
            if (!head) {
                head = node;
            } else {
                PlayerProfile* cur = head;
                while (cur->next) cur = cur->next;
                cur->next = node;
            }
            
            if (id > maxId) maxId = id;
        }
        
        nextId = maxId + 1;
        file.close();
    }

    /*
     * addPlayer()
     * CREATE: Menambah pemain baru ke linked list.
     * startChips = 100 (sesuai permintaan, bukan 500 lagi).
     */
    void addPlayer(string username, string password, int startChips = 100, bool silent = false) {
        PlayerProfile* node = new PlayerProfile(nextId++, username, password, startChips);
        
        if (!head) {
            head = node;
        } else {
            PlayerProfile* cur = head;
            while (cur->next) cur = cur->next;
            cur->next = node;
        }
        if (!silent)
            cout << "\n  [+] Pemain \"" << username << "\" ditambahkan (ID: " << node->id << ").\n";
        
        saveToFile();  // Simpan ke file
    }

    /*
     * isUsernameExists()
     * Mengecek apakah username sudah terdaftar (case insensitive).
     * "Rafah" dan "rafah" dianggap sama.
     */
    bool isUsernameExists(const string& username) {
        string lowerUsername = toLowercase(username);
        PlayerProfile* cur = head;
        while (cur) {
            if (toLowercase(cur->username) == lowerUsername) return true;
            cur = cur->next;
        }
        return false;
    }

    /*
     * login()
     * Verifikasi username (case insensitive) dan password (case sensitive).
     * Mengembalikan pointer ke PlayerProfile jika berhasil.
     */
    PlayerProfile* login(const string& username, const string& password) {
        string lowerUsername = toLowercase(username);
        PlayerProfile* cur = head;
        while (cur) {
            if (toLowercase(cur->username) == lowerUsername && cur->password == password) 
                return cur;
            cur = cur->next;
        }
        return nullptr;
    }

    /*
     * showLeaderboard()
     * READ: Menampilkan semua pemain diurutkan dari chips terbanyak ke tersedikit.
     * Menggunakan bubble sort pada array pointer.
     */
    void showLeaderboard() const {
        if (!head) { 
            cout << "\n  Tidak ada pemain yang terdaftar.\n"; 
            return; 
        }

        // Hitung jumlah pemain
        int count = 0;
        PlayerProfile* cur = head;
        while (cur) { count++; cur = cur->next; }

        // Salin pointer ke array
        PlayerProfile** arr = new PlayerProfile*[count];
        cur = head;
        for (int i = 0; i < count; i++) {
            arr[i] = cur;
            cur = cur->next;
        }

        // Bubble sort berdasarkan chips (terbanyak ke tersedikit)
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (arr[j]->chips < arr[j+1]->chips) {
                    PlayerProfile* temp = arr[j];
                    arr[j] = arr[j+1];
                    arr[j+1] = temp;
                }
            }
        }

        // Tampilkan leaderboard
        cout << "\n";
        cout << "  +------+----------------------+--------+--------+--------+\n";
        cout << "  | Rank | Username             | Chips  | Menang | Kalah  |\n";
        cout << "  +------+----------------------+--------+--------+--------+\n";

        for (int i = 0; i < count; i++) {
            cout << "  | "
                 << right << setw(4) << (i+1) << " | "
                 << left  << setw(20) << arr[i]->username << " | "
                 << right << setw(6)  << arr[i]->chips << " | "
                 << right << setw(6)  << arr[i]->wins  << " | "
                 << right << setw(6)  << arr[i]->losses << " |\n";
        }
        cout << "  +------+----------------------+--------+--------+--------+\n";

        delete[] arr;
    }

    /*
     * updatePlayerAccount() - DIPERBAIKI
     * UPDATE: Mengedit username dan password akun yang sedang login.
     * Validasi username tidak boleh sama dengan yang sudah ada.
     * Validasi password minimal 8 karakter dan konfirmasi sesuai.
     */
    void updatePlayerAccount(PlayerProfile* p) {
        if (!p) return;

        // Helper functions untuk box (lambda)
        auto bRow = [](const string& t) {
            const int W = 51;
            cout << "  | " << t;
            int pad = W - (int)t.size(); if (pad < 0) pad = 0;
            for (int i = 0; i < pad; i++) cout << " ";
            cout << "|\n";
        };
        auto bCenter = [](const string& t) {
            const int W = 51;
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

        int choice = 0;
        do {
            cout << "\n";
            bBorder();
            bCenter("EDIT AKUN: " + p->username);
            bBorder();
            bRow("  [ 1 ]  Ganti username");
            bRow("  [ 2 ]  Ganti password");
            bRow("  [ 3 ]  Ganti username & password");
            bRow("  [ 0 ]  Kembali");
            bBorder();
            cout << "  Pilih opsi: ";
            choice = getIntInput();
            if (choice < 0 || choice > 3) {
                bBorder();
                bCenter("[GAGAL] Pilihan tidak valid! Masukkan angka 0, 1, 2, atau 3.");
                bBorder();
                cout << "\n";
            }
        } while (choice < 0 || choice > 3);

        if (choice == 0) {
            return;
        }

        // Edit username
        if (choice == 1 || choice == 3) {
            string newUsername;
            while (true) {
                cout << "  Username baru (maks 15 karakter, 1 kata): ";
                newUsername = getUsernameInput();
                if (toLowercase(newUsername) != toLowercase(p->username) && isUsernameExists(newUsername)) {
                    printInlineError("Username '" + newUsername + "' sudah digunakan!");
                    continue;
                }
                break;
            }
            p->username = newUsername;
        }
        
        // Edit password
        if (choice == 2 || choice == 3) {
            string newPass, confirmPass;

            // biar pas input password itu gak kebaca kosong
            if (choice == 2) {
                 cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } 
            // Input password baru (diluar loop konfirmasi)
            cout << "  Password baru (minimal 8 karakter): ";
            newPass = getPasswordInput();
            
            // Loop konfirmasi password sampai sesuai
            while (true) {
                cout << "  Konfirmasi password baru: ";
                confirmPass = getConfirmPasswordInput();
                
                if (newPass != confirmPass) {
                    printInlineError("Konfirmasi password tidak sesuai!");
                    continue;
                }
                break;  // Keluar loop hanya jika password sesuai
            }
            p->password = newPass;
        }
        
        bBorder();
        bCenter("Akun berhasil diperbarui!");
        bBorder();
        cout << "\n";
        
        saveToFile();  // Simpan perubahan
    }

    /*
     * deletePlayer()
     * DELETE: Menghapus profil pemain dari linked list.
     * Mengembalikan true jika berhasil, false jika gagal.
     */
    bool deletePlayer(PlayerProfile* target) {
        if (!head || !target) return false;

        // Kasus 1: target adalah head
        if (head == target) {
            head = head->next;
            delete target;
            saveToFile();
            return true;
        }

        // Kasus 2: target di tengah atau akhir
        PlayerProfile* prev = head;
        while (prev->next && prev->next != target)
            prev = prev->next;

        if (!prev->next) return false;

        prev->next = target->next;
        delete target;
        saveToFile();
        return true;
    }

    // Destructor: membersihkan semua memori
    ~PlayerRegistry() { clear(); }

    // Membersihkan linked list
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

// ============================================================
// SECTION 3 — DECK (Array 1D + Manipulation Array Pointing)
// TUGAS ANGGOTA 2: Manajemen Array Konstan dan Randomisasi Angka
// ============================================================

const int DECK_SIZE = 52;
Card deck[DECK_SIZE];   // Array statis 52 kartu
int  deckIndex = 0;     // Indeks kartu berikutnya yang akan dibagikan

/*
 * buildDeck()
 * Menginisialisasi deck dengan 52 kartu lengkap.
 * 4 suit (Hearts, Diamonds, Clubs, Spades) x 13 rank (2-10,J,Q,K,A).
 */
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

/*
 * shuffleDeck()
 * Mengacak posisi kartu menggunakan algoritma Fisher-Yates.
 * Juga mereset deckIndex ke 0.
 */
void shuffleDeck() {
    for (int i = DECK_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card tmp = deck[i];
        deck[i] = deck[j];
        deck[j] = tmp;
    }
    deckIndex = 0;
}

/*
 * dealCard()
 * Mengambil satu kartu dari deck (mengembalikan pointer ke Card).
 * Jika deck habis, akan diacak ulang otomatis.
 */
Card* dealCard() {
    if (deckIndex >= DECK_SIZE) { 
        shuffleDeck(); 
        cout << "  [Deck diacak ulang]\n"; 
    }
    return &deck[deckIndex++];
}

// ============================================================
// SECTION 4 — UI HELPERS, TAMPILAN MEJA, & GAME LOGIC
// TUGAS ANGGOTA 5: Perancangan Arsitektur Aturan Game & Payout
// ============================================================

const int BOX = 52;  // Lebar box untuk tampilan

void printBorder() {
    cout << "  +";
    for (int i = 0; i < BOX; i++) cout << "-";
    cout << "+\n";
}

void printBorderThin() {
    cout << "  |";
    for (int i = 0; i < BOX; i++) cout << "-";
    cout << "|\n";
}

/*
 * printBoxRow()
 * Mencetak teks rata kiri di dalam box dengan word-wrap otomatis.
 */
void printBoxRow(const string& text) {
    const int W = BOX - 1;
    if ((int)text.size() <= W) {
        cout << "  | " << text;
        for (int i = 0; i < W - (int)text.size(); i++) cout << " ";
        cout << "|\n";
        return;
    }

    int indent = 0;
    while (indent < (int)text.size() && text[indent] == ' ') indent++;
    const string cont(indent + 2, ' ');

    string rem = text;
    bool first = true;
    while (!rem.empty()) {
        int avail = first ? W : (W - (int)cont.size());
        string pfx = first ? "" : cont;

        if ((int)rem.size() <= avail) {
            string line = pfx + rem;
            cout << "  | " << line;
            for (int i = 0; i < W - (int)line.size(); i++) cout << " ";
            cout << "|\n";
            break;
        }

        int cut = avail;
        while (cut > 0 && rem[cut] != ' ') cut--;
        if (cut == 0) cut = avail;

        string line = pfx + rem.substr(0, cut);
        cout << "  | " << line;
        for (int i = 0; i < W - (int)line.size(); i++) cout << " ";
        cout << "|\n";

        rem = rem.substr(cut);
        while (!rem.empty() && rem[0] == ' ') rem = rem.substr(1);
        first = false;
    }
}

/*
 * printBoxCenter()
 * Mencetak teks rata tengah di dalam box dengan word-wrap otomatis.
 */
void printBoxCenter(const string& text) {
    const int W = BOX - 1;
    if ((int)text.size() <= W) {
        int lpad = (W - (int)text.size()) / 2;
        int rpad = W - (int)text.size() - lpad;
        cout << "  | ";
        for (int i = 0; i < lpad; i++) cout << " ";
        cout << text;
        for (int i = 0; i < rpad; i++) cout << " ";
        cout << "|\n";
        return;
    }

    string rem = text;
    while (!rem.empty()) {
        int cut = (int)rem.size() <= W ? (int)rem.size() : W;
        if ((int)rem.size() > W) {
            while (cut > 0 && rem[cut] != ' ') cut--;
            if (cut == 0) cut = W;
        }
        string chunk = rem.substr(0, cut);
        int lpad = (W - (int)chunk.size()) / 2;
        int rpad = W - (int)chunk.size() - lpad;
        cout << "  | ";
        for (int i = 0; i < lpad; i++) cout << " ";
        cout << chunk;
        for (int i = 0; i < rpad; i++) cout << " ";
        cout << "|\n";
        rem = rem.substr(cut);
        while (!rem.empty() && rem[0] == ' ') rem = rem.substr(1);
    }
}

/*
 * printHandInBox()
 * Mencetak kartu-kartu secara horizontal dalam format ASCII art.
 * Setiap kartu ditampilkan dalam 5 baris.
 */
void printHandInBox(const Hand& hand, bool hideFirst) {
    const int MAX_CARDS = 11;
    string rows[5][MAX_CARDS];
    int count = 0;

    HandNode* cur = hand.head;
    int idx = 0;
    while (cur && count < MAX_CARDS) {
        bool hide = (hideFirst && idx == 0);
        string r = cur->card.rank;
        if (r.size() == 1) r += " ";
        char sc = cur->card.suit[0];

        if (hide) {
            rows[0][count] = ".------. ";
            rows[1][count] = "| ???? | ";
            rows[2][count] = "|  ??  | ";
            rows[3][count] = "| ???? | ";
            rows[4][count] = "'------' ";
        } else {
            rows[0][count] = ".------. ";
            rows[1][count] = "| " + r + "   | ";
            rows[2][count] = "|  " + string(1, sc) + "   | ";
            rows[3][count] = "|   " + r + " | ";
            rows[4][count] = "'------' ";
        }
        count++;
        cur = cur->next;
        idx++;
    }

    for (int row = 0; row < 5; row++) {
        string line = "";
        for (int c = 0; c < count; c++) {
            line += rows[row][c];
        }
        int pad = BOX - 1 - (int)line.size();
        if (pad < 0) pad = 0;
        cout << "  | " << line;
        for (int i = 0; i < pad; i++) cout << " ";
        cout << "|\n";
    }
}

/*
 * showGameTable()
 * Menampilkan seluruh meja permainan dalam satu box:
 * - Info pemain (nama, chips, taruhan)
 * - Kartu dealer (bisa disembunyikan)
 * - Kartu pemain
 * - Status permainan
 */
void showGameTable(PlayerProfile* p, const Hand& playerHand, const Hand& dealerHand,
                   bool hideDealer, int bet, const string& status = "") {
    cout << "\n";
    printBorder();
    printBoxCenter("-- BLACKJACK TABLE --");
    printBorder();

    string info = "Pemain: " + p->username + "   Chips: " + to_string(p->chips)
                + "   Taruhan: " + to_string(bet);
    printBoxRow(info);
    printBorder();

    if (hideDealer)
        printBoxRow("DEALER  [ 1 kartu tersembunyi ]");
    else
        printBoxRow("DEALER  [ Total: " + to_string(dealerHand.total()) + " ]");

    printHandInBox(dealerHand, hideDealer);
    printBorderThin();

    printBoxRow(p->username + "  [ Total: " + to_string(playerHand.total()) + " ]");
    printHandInBox(playerHand, false);
    printBorder();

    if (!status.empty()) {
        printBoxCenter(status);
        printBorder();
    }
    cout << "\n";
}

/*
 * showRules()
 * Menampilkan aturan bermain Blackjack.
 */
void showRules() {
    clearScreen();
    cout << "\n";
    printBorder();
    printBoxCenter("CARA BERMAIN BLACKJACK");
    printBorder();
    printBoxRow("TUJUAN:");
    printBoxRow("  Kalahkan dealer dengan total kartu sedekat 21,");
    printBoxRow("  tanpa melebihinya (BUST).");
    printBorderThin();
    printBoxRow("NILAI KARTU:");
    printBoxRow("  2 - 10  =  Sesuai angkanya");
    printBoxRow("  J, Q, K =  Bernilai 10");
    printBoxRow("  A (As)  =  Bernilai 11 atau 1 (otomatis)");
    printBorderThin();
    printBoxRow("ALUR PERMAINAN:");
    printBoxRow("  1. Pasang taruhan sebelum kartu dibagikan.");
    printBoxRow("  2. Kamu & dealer masing-masing dapat 2 kartu.");
    printBoxRow("     (1 kartu dealer disembunyikan)");
    printBoxRow("  3. Pilih aksi giliran kamu:");
    printBoxRow("     [H] HIT   = Minta 1 kartu tambahan");
    printBoxRow("     [S] STAND = Berhenti, giliran dealer");
    printBoxRow("  4. Dealer WAJIB HIT jika totalnya di bawah 17.");
    printBorderThin();
    printBoxRow("HASIL:");
    printBoxRow("  MENANG  = Total kamu > dealer (max 21)");
    printBoxRow("  KALAH   = Kamu BUST (>21) atau total < dealer");
    printBoxRow("  PUSH    = Seri, taruhan dikembalikan");
    printBorderThin();
    printBoxRow("BLACKJACK (Bonus!):");
    printBoxRow("  2 kartu pertama = 21 (As + 10/J/Q/K)");
    printBoxRow("  Itu BLACKJACK! Bayaran 3:2 (taruhan x 2.5)");
    printBorderThin();
    printBoxRow("PAYOUT:");
    printBoxRow("  Menang biasa  = Taruhan x 2");
    printBoxRow("  Blackjack     = Taruhan x 2.5  (rasio 3:2)");
    printBoxRow("  Push / Seri   = Taruhan dikembalikan");
    printBoxRow("  Bust / Kalah  = Taruhan hangus");
    printBorder();
    cout << "  Tekan ENTER untuk melanjutkan...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    waitEnter();
}

/*
 * showResult()
 * Menampilkan hasil akhir ronde permainan.
 */
void showResult(PlayerProfile* p, int pt, int dt, bool bust,
                bool playerBJ, bool dealerBJ, int bet, int payout, bool win, bool push) {
    cout << "\n";
    printBorder();
    printBoxCenter("HASIL RONDE");
    printBorder();
    printBoxRow("  Total kamu   : " + to_string(pt));
    printBoxRow("  Total dealer : " + to_string(dt));
    printBorderThin();

    if (bust)
        printBoxCenter("!! BUST !! Kamu melebihi 21 -- KALAH");
    else if (playerBJ && !dealerBJ)
        printBoxCenter("** BLACKJACK! ** Menang " + to_string(payout) + " chips (3:2)");
    else if (playerBJ && dealerBJ)
        printBoxCenter("Sama-sama Blackjack -- PUSH");
    else if (dt > 21)
        printBoxCenter("Dealer BUST! Kamu menang " + to_string(payout) + " chips");
    else if (win)
        printBoxCenter("Kamu MENANG! +" + to_string(payout) + " chips");
    else if (push)
        printBoxCenter("PUSH -- Seri. Taruhan dikembalikan.");
    else
        printBoxCenter("Kamu KALAH. Dealer menang.");

    printBorderThin();
    printBoxRow("  Chips sekarang : " + to_string(p->chips));
    printBoxRow("  Menang : " + to_string(p->wins) + "   Kalah : " + to_string(p->losses));
    printBorder();
    cout << "\n";
}

/*
 * playBlackjack()
 * Fungsi utama permainan Blackjack satu ronde.
 * Parameter p: pointer ke pemain yang sedang bermain.
 */
void playBlackjack(PlayerProfile* p) {
    Hand playerHand, dealerHand;
    int  bet = 0;
    char playAgain = 'y';

    while (playAgain == 'y' || playAgain == 'Y') {  // <-- TAMBAHKAN: loop utama
        Hand playerHand, dealerHand;
        int  bet = 0;

        cout << "\n";
        printBorder();
        printBoxCenter("PASANG TARUHAN");
        printBorder();
        printBoxRow("  Pemain : " + p->username);
        printBoxRow("  Chips  : " + to_string(p->chips));
        printBorder();
        do {
            cout << "  Masukkan taruhan (1-" << p->chips << ", 0 = batal): ";
            bet = getIntInput();
            if (bet == 0) { 
                playerHand.clear(); 
                dealerHand.clear(); 
                return; 
            }
            if (bet < 1 || bet > p->chips)
                cout << "  [GAGAL] Taruhan tidak valid. Coba lagi.\n";
        } while (bet < 1 || bet > p->chips);

        p->chips -= bet;

        // Pembagian kartu awal
        playerHand.addCard(*dealCard());
        dealerHand.addCard(*dealCard());
        playerHand.addCard(*dealCard());
        dealerHand.addCard(*dealCard());

        bool bust            = false;
        bool playerBlackjack = (playerHand.total() == 21 && playerHand.size == 2);
        bool dealerBJearly   = (dealerHand.total() == 21 && dealerHand.size == 2);

        showGameTable(p, playerHand, dealerHand, true, bet, "Kartu dibagikan! Giliran kamu.");

        // Giliran pemain
        if (!playerBlackjack) {
            char ch;
            do {
                printBorder();
                printBoxRow("  [H] Hit  = Minta kartu tambahan");
                printBoxRow("  [S] Stand = Berhenti, giliran dealer");
                printBorder();
                cout << "  Pilihan kamu (H/S): ";

                while (true) {
                    cin >> ch;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (ch == 'H' || ch == 'h' || ch == 'S' || ch == 's') break;
                    printInlineError("Input tidak valid. Ketik 'H' untuk Hit atau 'S' untuk Stand.");
                    cout << "  > ";
                }

                if (ch == 'H' || ch == 'h') {
                    playerHand.addCard(*dealCard());
                    if (playerHand.total() > 21) {
                        showGameTable(p, playerHand, dealerHand, true, bet,
                                      "BUST! Total kamu melebihi 21 -- KALAH!");
                        bust = true;
                        break;
                    }
                    showGameTable(p, playerHand, dealerHand, true, bet,
                                  "Kartu ditambahkan. Total: " + to_string(playerHand.total()));
                } else {
                    showGameTable(p, playerHand, dealerHand, true, bet, "Kamu STAND. Giliran dealer...");
                }
            } while (ch != 'S' && ch != 's' && !bust);
        } else {
            showGameTable(p, playerHand, dealerHand, true, bet, "BLACKJACK! Kartu sempurna!");
        }

        // Giliran dealer
        if (!bust) {
            showGameTable(p, playerHand, dealerHand, false, bet, "Dealer membuka kartu tersembunyi...");
            while (dealerHand.total() < 17) {
                dealerHand.addCard(*dealCard());
                string st = (dealerHand.total() > 21)
                    ? "Dealer BUST! Total: " + to_string(dealerHand.total())
                    : "Dealer HIT. Total: " + to_string(dealerHand.total());
                showGameTable(p, playerHand, dealerHand, false, bet, st);
            }
            if (dealerHand.total() <= 21)
                showGameTable(p, playerHand, dealerHand, false, bet,
                              "Dealer STAND. Total: " + to_string(dealerHand.total()));
        }

        // Hitung hasil
        int  pt       = playerHand.total();
        int  dt       = dealerHand.total();
        bool dealerBJ = dealerBJearly;
        bool win      = false;
        bool push     = false;
        int  payout   = 0;

        if (bust) {
            p->losses++;
        } else if (playerBlackjack && !dealerBJ) {
            payout = bet + bet * 3 / 2;
            p->chips += payout;
            p->wins++;
            win = true;
        } else if (playerBlackjack && dealerBJ) {
            p->chips += bet;
            push = true;
        } else if (dt > 21 || pt > dt) {
            payout = bet * 2;
            p->chips += payout;
            p->wins++;
            win = true;
        } else if (pt == dt) {
            p->chips += bet;
            push = true;
        } else {
            p->losses++;
        }

        showResult(p, pt, dt, bust, playerBlackjack, dealerBJ, bet, payout, win, push);

        playerHand.clear();
        dealerHand.clear();

        if (deckIndex > DECK_SIZE * 3 / 4) {
            shuffleDeck();
            cout << "  [Deck diacak ulang]\n\n";
        }
        
                // Tanya apakah ingin bermain lagi
        if (p->chips > 0) {
            cout << "\n";
            printBorder();
            printBoxCenter("Main lagi?");
            printBorder();
            printBoxRow("  Chips kamu sekarang: " + to_string(p->chips));
            printBorder();
            cout << "  (y/n): ";
            playAgain = getYesNoInput();
            
            if (playAgain == 'y' || playAgain == 'Y') {
                clearScreen();
                continue;
            }
        } else {
            // CHIPS HABIS - TANYA BELI CHIPS
            cout << "\n";
            printBorder();
            printBoxCenter("[GAGAL] " + p->username + " tidak punya chips. apakah ingin membeli chips?");
            printBorder();
            cout << "  (y/n): ";
            char buyChips = getYesNoInput();
            
            if (buyChips == 'y' || buyChips == 'Y') {
                cout << "  Masukkan jumlah chips baru (1-999999): ";
                int newChips = getChipsInput();
                p->chips = newChips;
                cout << "\n";
                printBorder();
                printBoxCenter("Berhasil membeli " + to_string(newChips) + " chips!");
                printBorder();
                cout << "\n  Tekan ENTER untuk melanjutkan bermain...";
                waitEnter();
                clearScreen();
                continue;  // LANJUT RONDE BERIKUTNYA
            } else {
                playAgain = 'n';
            }
        }   
    }
}

// ============================================================
// SECTION 5 — MENUS UTAMA & ALUR APP
// TUGAS ANGGOTA 1: Mengatur Rute (Router) Program Secara Menyeluruh
// ============================================================

/*
 * printWelcomeBanner()
 * Menampilkan banner ASCII art saat program pertama kali dijalankan.
 */
void printWelcomeBanner() {
    cout << "\n";
    cout << "  ██████╗ ██╗      █████╗  ██████╗██╗  ██╗     ██╗ █████╗  ██████╗██╗  ██╗\n";
    cout << "  ██╔══██╗██║     ██╔══██╗██╔════╝██║ ██╔╝     ██║██╔══██╗██╔════╝██║ ██╔╝\n";
    cout << "  ██████╔╝██║     ███████║██║     █████╔╝      ██║███████║██║     █████╔╝ \n";
    cout << "  ██╔══██╗██║     ██╔══██║██║     ██╔═██╗ ██   ██║██╔══██║██║     ██╔═██╗ \n";
    cout << "  ██████╔╝███████╗██║  ██║╚██████╗██║  ██╗╚█████╔╝██║  ██║╚██████╗██║  ██╗\n";
    cout << "  ╚═════╝ ╚══════╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝ ╚════╝ ╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝\n";
    cout << "                              By QuintAce";
    cout << "\n";
}

/*
 * printMainMenu()
 * Menampilkan menu utama (Tambah Pemain, Login, Papan Peringkat, Keluar).
 */
void printMainMenu() {
    cout << "\n";
    printBorder();
    printBoxCenter("M E N U   U T A M A");
    printBorder();
    printBoxRow("  [ 1 ]  Tambah Pemain Baru");
    printBorderThin();
    printBoxRow("  [ 2 ]  Login");
    printBorderThin();
    printBoxRow("  [ 3 ]  Papan Peringkat");
    printBorderThin();
    printBoxRow("  [ 0 ]  Keluar");
    printBorder();
    cout << "  Pilih menu: ";
}

/*
 * printGameMenu()
 * Menampilkan menu permainan setelah login berhasil.
 */
void printGameMenu(const string& username) {
    cout << "\n";
    printBorder();
    printBoxCenter("SELAMAT DATANG, " + username);
    printBorder();
    printBoxRow("  [ 1 ]  Mulai Bermain");
    printBorderThin();
    printBoxRow("  [ 2 ]  Aturan Permainan");
    printBorderThin();
    printBoxRow("  [ 3 ]  Manajemen Akun");
    printBorderThin();
    printBoxRow("  [ 4 ]  Logout");
    printBorder();
    cout << "  Pilih menu: ";
}

/*
 * printSuccessMsg()
 * Menampilkan pesan sukses dalam box.
 */
void printSuccessMsg(const string& msg) {
    cout << "\n";
    printBorder();
    printBoxCenter(msg);
    printBorder();
    cout << "\n";
}

/*
 * printErrorMsg()
 * Menampilkan pesan error dalam box.
 */
void printErrorMsg(const string& msg) {
    cout << "\n";
    printBorder();
    printBoxCenter("[GAGAL] " + msg);
    printBorder();
    cout << "\n";
}

/*
 * printInfoMsg()
 * Menampilkan pesan informasi dalam box tipis.
 */
void printInfoMsg(const string& msg) {
    cout << "\n";
    printBorderThin();
    printBoxCenter(msg);
    printBorderThin();
    cout << "\n";
}

/*
 * addNewPlayer() - DIPERBAIKI
 * Menu untuk menambah pemain baru.
 * Validasi: username unik (case insensitive), password minimal 8 karakter,
 * konfirmasi password sesuai. Chips awal = 100.
 * 
 * PERBAIKAN: Membersihkan buffer dengan benar sebelum input password.
 */
void addNewPlayer(PlayerRegistry& reg) {
    clearScreen();
    cout << "\n";
    printBorder();
    printBoxCenter("TAMBAH PEMAIN BARU");
    printBorder();
    
    string username, password, confirmPass;
    
    // Input username - loop sampai username tidak terdaftar
    while (true) {
        cout << "  Username (maks 15 karakter, 1 kata): ";
        username = getUsernameInput();
        
        if (reg.isUsernameExists(username)) {
            printInlineError("Username '" + username + "' sudah digunakan!");
            continue;
        }
        break;
    }
    
    // Input password - loop sampai valid (minimal 8 karakter)
    while (true) {
        cout << "  Password (minimal 8 karakter): ";
        password = getPasswordInput();
        break;  // getPasswordInput sudah memvalidasi
    }
    
    // Input konfirmasi password - loop sampai sesuai dengan password
    while (true) {
        cout << "  Konfirmasi password: ";
        confirmPass = getConfirmPasswordInput();
        
        if (password != confirmPass) {
            printInlineError("Konfirmasi password tidak sesuai!");
            continue;
        }
        break;
    }
    
    // Tambah pemain dengan chips awal 100
    reg.addPlayer(username, password, 100);
    printSuccessMsg("Pemain \"" + username + "\" berhasil ditambahkan dengan 100 chips!");
    cout << "  Tekan ENTER untuk kembali...";
    waitEnter();
    clearScreen();
}

/*
 * loginMenu() - DIPERBAIKI
 * Proses login: input username dan password.
 * Username case insensitive, password case sensitive.
 * Jika salah, akan looping di dalam fungsi ini.
 * 
 * PERBAIKAN: Membersihkan buffer dengan benar sebelum input password.
 */
PlayerProfile* loginMenu(PlayerRegistry& reg) {
    clearScreen();
    cout << "\n";
    printBorder();
    printBoxCenter("LOGIN");
    printBorder();
    
    string username, password;
    
    // Input username - loop sampai username terdaftar
    while (true) {
        cout << "  Username: ";
        username = getUsernameInput();
        
        if (!reg.isUsernameExists(username)) {
            printInlineError("Username '" + username + "' tidak terdaftar!");
            continue;
        }
        break;
    }
    
    // Input password - loop sampai password benar
    while (true) {
        cout << "  Password: ";
        password = getConfirmPasswordInput();
        
        PlayerProfile* p = reg.login(username, password);
        
        if (!p) {
            printInlineError("Password salah!");
            continue;
        }
        
        printSuccessMsg("Login berhasil! Selamat datang, " + p->username + "!");
        return p;
    }
}

/*
 * accountManagementMenu() - DIPERBAIKI
 * Manajemen akun untuk pemain yang sedang login.
 * Bisa edit username/password atau hapus akun.
 */
void accountManagementMenu(PlayerRegistry& reg, PlayerProfile*& currentPlayer) {
    if (!currentPlayer) return;
    
    int choice;
    do {
        clearScreen();
        cout << "\n";
        printBorder();
        printBoxCenter("MANAJEMEN AKUN");
        printBorder();
        printBoxRow("  Username: " + currentPlayer->username);
        printBoxRow("  Chips   : " + to_string(currentPlayer->chips));
        printBorderThin();
        printBoxRow("  [ 1 ]  Edit Username & Password");
        printBorderThin();
        printBoxRow("  [ 2 ]  Hapus Akun");
        printBorderThin();
        printBoxRow("  [ 0 ]  Kembali");
        printBorder();
        cout << "  Pilih menu: ";
        choice = getMenuInput(0, 2);
        
        if (choice == 1) {
            clearScreen();
            reg.updatePlayerAccount(currentPlayer);
            cout << "  Tekan ENTER untuk kembali...";
            waitEnter();
            
        } else if (choice == 2) {
            clearScreen();
            printBorder();
            printBoxCenter("KONFIRMASI HAPUS AKUN");
            printBorder();
            printBoxRow("  Apakah Anda yakin ingin menghapus akun \"" + currentPlayer->username + "\"?");
            printBorder();
            cout << "  (y/n): ";
            char confirm = getYesNoInput();
            
            if (confirm == 'y' || confirm == 'Y') {
                string deletedName = currentPlayer->username;
                bool success = reg.deletePlayer(currentPlayer);
                if (success) {
                    currentPlayer = nullptr;
                    printSuccessMsg("Akun \"" + deletedName + "\" berhasil dihapus!");
                    cout << "  Tekan ENTER untuk melanjutkan...";
                    waitEnter();
                    clearScreen();
                    return;
                } else {
                    printErrorMsg("Gagal menghapus akun!");
                }
            } else {
                printInfoMsg("Penghapusan akun dibatalkan.");
            }
            cout << "  Tekan ENTER untuk kembali...";
            waitEnter();
        }
        // choice == 0 -> tidak perlu ditangani, loop akan berhenti karena while (choice != 0)
        
    } while (choice != 0);
}

/*
 * playGameMenu()
 * Menu permainan setelah login:
 * - Mulai Bermain: langsung ke playBlackjack()
 * - Aturan Permainan: tampilkan aturan
 * - Manajemen Akun: edit/hapus akun
 * - Logout: kembali ke menu utama
 */
void playGameMenu(PlayerRegistry& reg, PlayerProfile*& currentPlayer) {
    if (!currentPlayer) return;
    
    int choice;
    do {
        clearScreen();
        printGameMenu(currentPlayer->username);
        choice = getMenuInput(0, 4);
        
        if (choice == 1) {  // Mulai Bermain
            clearScreen();
            if (currentPlayer->chips <= 0) {
                printErrorMsg(currentPlayer->username + " tidak punya chips. apakah ingin membeli chips?");
                cout << "  (y/n): ";
                char addChips = getYesNoInput();
                if (addChips == 'y' || addChips == 'Y') {
                    cout << "  Masukkan jumlah chips (1-999999): ";
                    int newChips = getChipsInput();
                    currentPlayer->chips = newChips;
                    reg.saveToFile();
                    printSuccessMsg("Chips " + currentPlayer->username + " sekarang: " + to_string(newChips));
                }
            } else {
                playBlackjack(currentPlayer);
                reg.saveToFile();  // Simpan perubahan
            }
            
        } else if (choice == 2) {  // Aturan Permainan
            showRules();
            
        } else if (choice == 3) {  // Manajemen Akun
            accountManagementMenu(reg, currentPlayer);
            
        } else if (choice == 4) {  // Logout
            cout << "\n";
            printBorder();
            printBoxCenter("KONFIRMASI LOGOUT");
            printBorder();
            cout << "  Apakah Anda yakin ingin logout? (y/n): ";
            char confirm = getYesNoInput();
            if (confirm == 'y' || confirm == 'Y') {
                printInfoMsg("Logout berhasil! Sampai jumpa, " + currentPlayer->username + ".");
                cout << "  Tekan ENTER untuk melanjutkan...";
                waitEnter();
                clearScreen();
                currentPlayer = nullptr;
                return;
            } else {
                printInfoMsg("Logout dibatalkan.");
                cout << "  Tekan ENTER untuk kembali...";
                waitEnter();
            }
        }
        // choice == 0 tidak perlu ditangani karena akan keluar dari loop
    
    } while (choice != 0 && currentPlayer != nullptr);
}

// ============================================================
// FUNGSI MAIN - ENTRY POINT PROGRAM
// ============================================================

int main() {
    // Inisialisasi random number generator
    srand(static_cast<unsigned>(time(nullptr)));

    // Inisialisasi deck kartu
    buildDeck();
    shuffleDeck();
    
    // Registry akan otomatis memuat data dari file "players.dat"
    PlayerRegistry registry;

    // Cek apakah file data sudah ada
    ifstream testFile("players.dat");
    if (!testFile.is_open()) {
        // File tidak ada, tambahkan data dummy
        registry.addPlayer("Rafah", "password123", 200, true);
        registry.addPlayer("Najwa", "password123", 150, true);
        registry.addPlayer("Indra", "password123", 100, true);
        registry.addPlayer("Aghniya", "password123", 120, true);
        registry.addPlayer("Fadhil", "password123", 50, true);
        registry.saveToFile();
    }
    testFile.close();

    clearScreen();
    printWelcomeBanner();

    int choice;
    PlayerProfile* currentPlayer = nullptr;
    
    do {
        if (currentPlayer == nullptr) {
            printMainMenu();
            choice = getMenuInput(0, 3);
            
            switch (choice) {
                case 1:  // Tambah Pemain Baru
                    addNewPlayer(registry);
                    break;
                    
                case 2:  // Login
                    currentPlayer = loginMenu(registry);
                    break;
                    
                case 3:  // Papan Peringkat
                    clearScreen();
                    cout << "\n";
                    printBorder();
                    printBoxCenter("P A P A N   P E R I N G K A T");
                    printBorder();
                    registry.showLeaderboard();
                    cout << "  Tekan ENTER untuk kembali...";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    waitEnter();
                    clearScreen();
                    break;
                    
                case 0:  // Keluar - dengan konfirmasi
                    cout << "\n";
                    printBorder();
                    printBoxCenter("KONFIRMASI KELUAR");
                    printBorder();
                    cout << "  Apakah Anda yakin ingin keluar dari program? (y/n): ";
                    char confirm = getYesNoInput();
                    if (confirm != 'y' && confirm != 'Y') {
                        choice = -1;  // Batalkan keluar, kembali ke menu
                        printInfoMsg("Keluar dibatalkan.");
                        cout << "  Tekan ENTER untuk kembali...";
                        waitEnter();
                    }
                    break;
            }
        } else {
            playGameMenu(registry, currentPlayer);
        }
        
    } while (choice != 0);

    // Tampilkan ringkasan sebelum keluar
    clearScreen();
    cout << "\n\n";
    printBorder();
    printBoxRow("");
    printBoxCenter("Terima kasih telah bermain Blackjack!");
    printBoxRow("");
    printBoxCenter(".------. .------.");
    printBoxCenter("| I    | | A    |");
    printBoxCenter("|  R   | |  N   |");
    printBoxCenter("|    F | |    5 |");
    printBoxCenter("'------' '------'");
    printBoxCenter("Q U I N T A C E !");
    printBoxRow("");
    printBorder();

    if (registry.head) {
        cout << "\n";
        printBorder();
        printBoxCenter("RINGKASAN SESI INI");
        printBorder();
        printBoxRow("  Username           Chips     Menang  Kalah");
        printBorderThin();
        PlayerProfile* cur = registry.head;
        while (cur) {
            string nama = cur->username;
            while ((int)nama.size() < 18) nama += " ";
            string chips = to_string(cur->chips);
            while ((int)chips.size() < 9)  chips += " ";
            string wins  = to_string(cur->wins);
            while ((int)wins.size()  < 7)  wins  += " ";
            printBoxRow("  " + nama + " " + chips + wins + to_string(cur->losses));
            cur = cur->next;
        }
        printBorder();
    }

    return 0;
}