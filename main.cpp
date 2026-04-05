// ═══════════════════════════════════════════════════════
//  MAIN
// ═══════════════════════════════════════════════════════

#ifndef MAIN
#define MAIN

// #include <windows.h>
#include "utility.h"

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

#endif