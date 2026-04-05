#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "utility.h"

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
#endif