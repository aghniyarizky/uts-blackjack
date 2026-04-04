// ═══════════════════════════════════════════════════════
//  FUNGSI PUSAT  / ENTRY POINT (MAIN)
// ═══════════════════════════════════════════════════════

#ifndef MAIN
#define MAIN

#include <windows.h>
#include "utility.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    srand(static_cast<unsigned>(time(nullptr)));

    buildDeck();    // Generate 52 kartu ke dalam array
    shuffleDeck();  // Acak posisi kartu dengan Fisher-Yates

    PlayerRegistry registry;

    registry.addPlayer("Rafah", 200, true);
    registry.addPlayer("Najwa", 150, true);
    registry.addPlayer("Indra", 100, true);
    registry.addPlayer("Aghniya", 120, true);
    registry.addPlayer("Fadhil", 50, true);

    printWelcomeBanner();

    int choice;
    do {
        printMainMenu();
        choice = getMenuInput(0, 2);

        if      (choice == 1) playerManagementMenu(registry);
        else if (choice == 2) playGameMenu(registry);

    } while (choice != 0);

    printExitScreen(registry);

    return 0; 
}

#endif