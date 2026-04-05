#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iomanip>
#include <fstream>     
#include <algorithm>    
#include <cctype>       
#include <limits>       
using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void waitEnter() {
    cin.get();
}

string toLowercase(string str) {
    for (int i = 0; i < (int)str.length(); i++) {
        str[i] = tolower(str[i]);
    }
    return str;
}

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

int getIntInput() {
    int val;
    while (!(cin >> val)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        printInlineError("Input tidak valid. Masukkan angka yang benar.");
        cout << "  > ";
    }
    return val;
}

char getYesNoInput() {
    char c;
    while (true) {
        cin >> c;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (c == 'y' || c == 'Y' || c == 'n' || c == 'N') return c;
        printInlineError("Input tidak valid. Masukkan 'y' atau 'n'.");
        cout << "  > ";
    }
}

string getUsernameInput() {
    const int MAX_NAME = 15;
    string name;
    while (true) {
        cin >> name;
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

string getPasswordInput() {
    const int MIN_PASS = 8;
    string pass;
    while (true) {
        getline(cin, pass);
        
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

string getConfirmPasswordInput() {
    string pass;
    while (true) {
        getline(cin, pass);
        
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

// ============================================================
// SECTION 2 — PLAYER REGISTRY (Linked list CRUD + File I/O)
// TUGAS ANGGOTA 4: Single Linked List Profil Global & Operasi CRUD
// ============================================================

struct PlayerProfile {
    int id;
    string username;    //case insensitive untuk login
    string password;    // case sensitive
    int chips;      
    int wins;       
    int losses;      
    PlayerProfile* next;       

    PlayerProfile(int i, string u, string p, int c)
        : id(i), username(u), password(p), chips(c), wins(0), losses(0), next(nullptr) {}
};

struct PlayerRegistry {
    PlayerProfile* head;
    int nextId;
    string filename;   

    PlayerRegistry(const string& fname = "players.dat") 
        : head(nullptr), nextId(1), filename(fname) {
        loadFromFile();
    }

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

    void loadFromFile() {
        ifstream file(filename);
        if (!file.is_open()) return;
        
        clear(); 
        
        string line;
        int maxId = 0;
        
        while (getline(file, line)) {
            if (line.empty()) continue;
            
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
        
        saveToFile();  
    }

    bool isUsernameExists(const string& username) {
        string lowerUsername = toLowercase(username);
        PlayerProfile* cur = head;
        while (cur) {
            if (toLowercase(cur->username) == lowerUsername) return true;
            cur = cur->next;
        }
        return false;
    }

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

    void showLeaderboard() const {
        if (!head) { 
            cout << "\n  Tidak ada pemain yang terdaftar.\n"; 
            return; 
        }

        int count = 0;
        PlayerProfile* cur = head;
        while (cur) { count++; cur = cur->next; }

        PlayerProfile** arr = new PlayerProfile*[count];
        cur = head;
        for (int i = 0; i < count; i++) {
            arr[i] = cur;
            cur = cur->next;
        }

        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (arr[j]->chips < arr[j+1]->chips) {
                    PlayerProfile* temp = arr[j];
                    arr[j] = arr[j+1];
                    arr[j+1] = temp;
                }
            }
        }

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

    void updatePlayerAccount(PlayerProfile* p) {
        if (!p) return;

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
        
        if (choice == 2 || choice == 3) {
            string newPass, confirmPass;

            if (choice == 2) {
                 cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } 
            cout << "  Password baru (minimal 8 karakter): ";
            newPass = getPasswordInput();
            
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

    bool deletePlayer(PlayerProfile* target) {
        if (!head || !target) return false;

        if (head == target) {
            head = head->next;
            delete target;
            saveToFile();
            return true;
        }

        PlayerProfile* prev = head;
        while (prev->next && prev->next != target)
            prev = prev->next;

        if (!prev->next) return false;

        prev->next = target->next;
        delete target;
        saveToFile();
        return true;
    }

    ~PlayerRegistry() { clear(); }

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

// ============================================================
// SECTION 4 — UI HELPERS, TAMPILAN MEJA, & GAME LOGIC
// TUGAS ANGGOTA 5: Perancangan Arsitektur Aturan Game & Payout
// ============================================================

const int BOX = 52; 

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

void playBlackjack(PlayerProfile* p) {
    Hand playerHand, dealerHand;
    int  bet = 0;
    char playAgain = 'y';

    while (playAgain == 'y' || playAgain == 'Y') {
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
        
        // apakah mau bermain lagi
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
            // chips habis - tanya beli chips
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
                continue;  // lanjut ke ronde selanjutnya
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

void printWelcomeBanner() {
    system("chcp 65001 > nul");
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

void printSuccessMsg(const string& msg) {
    cout << "\n";
    printBorder();
    printBoxCenter(msg);
    printBorder();
    cout << "\n";
}

void printErrorMsg(const string& msg) {
    cout << "\n";
    printBorder();
    printBoxCenter("[GAGAL] " + msg);
    printBorder();
    cout << "\n";
}

void printInfoMsg(const string& msg) {
    cout << "\n";
    printBorderThin();
    printBoxCenter(msg);
    printBorderThin();
    cout << "\n";
}

void addNewPlayer(PlayerRegistry& reg) {
    clearScreen();
    cout << "\n";
    printBorder();
    printBoxCenter("TAMBAH PEMAIN BARU");
    printBorder();
    
    string username, password, confirmPass;
    
    while (true) {
        cout << "  Username (maks 15 karakter, 1 kata): ";
        username = getUsernameInput();
        
        if (reg.isUsernameExists(username)) {
            printInlineError("Username '" + username + "' sudah digunakan!");
            continue;
        }
        break;
    }
    
    while (true) {
        cout << "  Password (minimal 8 karakter): ";
        password = getPasswordInput();
        break; 
    }
    
    while (true) {
        cout << "  Konfirmasi password: ";
        confirmPass = getConfirmPasswordInput();
        
        if (password != confirmPass) {
            printInlineError("Konfirmasi password tidak sesuai!");
            continue;
        }
        break;
    }
    
    reg.addPlayer(username, password, 100);
    printSuccessMsg("Pemain \"" + username + "\" berhasil ditambahkan dengan 100 chips!");
    cout << "  Tekan ENTER untuk kembali...";
    waitEnter();
    clearScreen();
}

PlayerProfile* loginMenu(PlayerRegistry& reg) {
    clearScreen();
    cout << "\n";
    printBorder();
    printBoxCenter("LOGIN");
    printBorder();
    
    string username, password;
    
    while (true) {
        cout << "  Username: ";
        username = getUsernameInput();
        
        if (!reg.isUsernameExists(username)) {
            printInlineError("Username '" + username + "' tidak terdaftar!");
            continue;
        }
        break;
    }
    
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
        
    } while (choice != 0);
}

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
    
    } while (choice != 0 && currentPlayer != nullptr);
}

// ============================================================
// FUNGSI MAIN - ENTRY POINT PROGRAM
// ============================================================

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    buildDeck();
    shuffleDeck();
    
    PlayerRegistry registry;

    ifstream testFile("players.dat");
    if (!testFile.is_open()) {
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
                        choice = -1;  // Batal keluar, kembali ke menu
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