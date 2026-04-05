// ============================================================
// SECTION 2 — PLAYER (Linked list CRUD + File I/O)
// ============================================================

#ifndef PLAYER_MNG
#define PLAYER_MNG

#include "utility.h"

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
                break;
            }
            p->password = newPass;
        }
        
        bBorder();
        bCenter("Akun berhasil diperbarui!");
        bBorder();
        cout << "\n";
        
        saveToFile();
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

#endif
