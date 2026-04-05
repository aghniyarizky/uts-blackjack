// ============================================================
// SECTION 5 — MENU UTAMA & ALUR GAME
// ============================================================

#ifndef ROUTE_MENU
#define ROUTE_MENU

#include "utility.h"

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
                    printBorder();
                    printBoxCenter("Berhasil membeli " + to_string(newChips) + " chips!");
                    printBorder();
                    cout << "\n  Tekan ENTER untuk melanjutkan bermain...";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin.get();
                    clearScreen();
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

#endif