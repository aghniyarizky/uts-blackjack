#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

// using namespace std;
#include "utility.h"

// Gunakan 'inline' agar bisa didefinisikan langsung di dalam header
inline void printInlineError(const string& msg) {
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

inline int getIntInput() {
    int val;
    while (!(cin >> val)) {
        cin.clear();
        while (cin.get() != '\n') continue;
        printInlineError("Input tidak valid. Masukkan angka yang benar.");
        cout << "  > ";
    }
    while (cin.get() != '\n') continue;
    return val;
}

inline char getYesNoInput() {
    char c;
    while (true) {
        cin >> c;
        while (cin.get() != '\n') continue;
        if (c == 'y' || c == 'Y' || c == 'n' || c == 'N') return c;
        printInlineError("Input tidak valid. Masukkan 'y' atau 'n'.");
        cout << "  > ";
    }
}

inline string getNameInput() {
    const int MAX_NAME = 15;
    string name;
    while (true) {
        cin >> name;
        while (cin.get() != '\n') continue;
        if (name.empty()) {
            printInlineError("Nama tidak boleh kosong.");
            cout << "  > ";
            continue;
        }
        if ((int)name.size() > MAX_NAME) {
            printInlineError("Nama terlalu panjang! Maksimal " + to_string(MAX_NAME) +
                             " karakter (kamu memasukkan " + to_string((int)name.size()) + ").");
            cout << "  > ";
            continue;
        }
        break;
    }
    return name;
}

inline int getChipsInput() {
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

#endif