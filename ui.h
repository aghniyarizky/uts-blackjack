// ═══════════════════════════════════════════════════════
//  SECTION 4 — UI HELPERS, TAMPILAN MEJA, & GAME LOGIC
// ═══════════════════════════════════════════════════════

#ifndef UI
#define UI

#include "utility.h"

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

        // Giliran player
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
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                clearScreen();
                continue;  // lanjut ke ronde selanjutnya
            } else {
                playAgain = 'n';
            }
        }   
    }
}

#endif
