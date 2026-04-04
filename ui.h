
// ═══════════════════════════════════════════════════════
//  SECTION 4 — UI HELPERS, TAMPILAN MEJA, & GAME LOGIC
//  TUGAS ANGGOTA 5: Perancangan Arsitektur Aturan Game & Payout
// ═══════════════════════════════════════════════════════

#ifndef UI
#define UI

#include "utility.h"

const int BOX = 52; // lebar dash border; konten maks = BOX-1 = 51

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
    // Kapasitas konten per baris di dalam box
    // Rumus: total_lebar(56) - prefix("  | "=4) - suffix("|"=1) = 51 = BOX - 1
    const int W = BOX - 1; // 51

    // ── Kasus 1: teks muat dalam satu baris ──
    if ((int)text.size() <= W) {
        cout << "  | " << text;
        for (int i = 0; i < W - (int)text.size(); i++) cout << " ";
        cout << "|\n";
        return;
    }

    // ── Kasus 2: word-wrap ──
    // Hitung indentasi awal (jumlah spasi di depan) untuk dipakai di baris lanjutan
    int indent = 0;
    while (indent < (int)text.size() && text[indent] == ' ') indent++;
    // Baris lanjutan: indentasi asli + 2 spasi ekstra sebagai penanda sambungan
    const string cont(indent + 2, ' ');

    string rem = text;   // sisa teks yang belum dicetak
    bool first = true;
    while (!rem.empty()) {
        // Lebar yang tersedia untuk baris ini
        int avail = first ? W : (W - (int)cont.size());
        string pfx = first ? "" : cont;

        if ((int)rem.size() <= avail) {
            // Sisa muat — cetak dan selesai
            string line = pfx + rem;
            cout << "  | " << line;
            for (int i = 0; i < W - (int)line.size(); i++) cout << " ";
            cout << "|\n";
            break;
        }

        // Cari posisi potong terakhir di batas kata (tidak memotong kata)
        int cut = avail;
        while (cut > 0 && rem[cut] != ' ') cut--;
        if (cut == 0) cut = avail; // paksa potong jika satu kata sangat panjang

        string line = pfx + rem.substr(0, cut);
        cout << "  | " << line;
        for (int i = 0; i < W - (int)line.size(); i++) cout << " ";
        cout << "|\n";

        rem = rem.substr(cut);
        // Buang spasi di awal sisa
        while (!rem.empty() && rem[0] == ' ') rem = rem.substr(1);
        first = false;
    }
}

void printBoxCenter(const string& text) {
    const int W = BOX - 1; // 51

    // ── Kasus 1: teks muat dalam satu baris ──
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

    // ── Kasus 2: word-wrap, tiap potongan di-tengahkan ──
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

    // 5 baris x MAX_CARDS kolom kartu
    string rows[5][MAX_CARDS];
    int count = 0;

    HandNode* cur = hand.head;
    int idx = 0;
    while (cur && count < MAX_CARDS) {
        bool hide = (hideFirst && idx == 0);

        // Rank selalu 2 karakter: "10", "K ", "A ", "2 ", dll
        string r = cur->card.rank;
        if (r.size() == 1) r += " "; // pad ke 2 char

        char sc = cur->card.suit[0]; // H, D, C, S

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

    // Cetak 5 baris secara horizontal, dibungkus dalam box
    for (int row = 0; row < 5; row++) {
        string line = "";
        for (int c = 0; c < count; c++) {
            line += rows[row][c];
        }
        // Cetak baris di dalam box dengan padding kanan otomatis
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

    // Info pemain
    // Buat string manual agar lebar bisa dikontrol
    string info = "Pemain: " + p->name + "   Chips: " + to_string(p->chips)
                + "   Taruhan: " + to_string(bet);
    printBoxRow(info);
    printBorder();

    // ── Dealer ──
    if (hideDealer)
        printBoxRow("DEALER  [ 1 kartu tersembunyi ]");
    else
        printBoxRow("DEALER  [ Total: " + to_string(dealerHand.total()) + " ]");

    printHandInBox(dealerHand, hideDealer);
    printBorderThin();

    // ── Pemain ──
    printBoxRow(p->name + "  [ Total: " + to_string(playerHand.total()) + " ]");
    printHandInBox(playerHand, false);
    printBorder();

    // Status message
    if (!status.empty()) {
        printBoxCenter(status);
        printBorder();
    }
    cout << "\n";
}

void showRules() {
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
    cout << "  Tekan ENTER untuk mulai bermain...";
    while (cin.get() != '\n') continue;
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

// ── Fungsi utama satu ronde Blackjack ──────────────────────────
void playBlackjack(PlayerProfile* p) {
    Hand playerHand, dealerHand; // Menyiapkan 2 Object Struct Linked List Kosong
    int  bet = 0;

    // ── 1. TEMPATKAN TARUHAN ──
    cout << "\n";
    printBorder();
    printBoxCenter("PASANG TARUHAN");
    printBorder();
    printBoxRow("  Pemain : " + p->name);
    printBoxRow("  Chips  : " + to_string(p->chips));
    printBorder();
    do {
        cout << "  Masukkan taruhan (1-" << p->chips << ", 0 = batal): ";
        bet = getIntInput();
        if (bet == 0) { playerHand.clear(); dealerHand.clear(); return; }
        if (bet < 1 || bet > p->chips)
            cout << "  [GAGAL] Taruhan tidak valid. Coba lagi.\n";
    } while (bet < 1 || bet > p->chips);

    p->chips -= bet; // Taruhan ditahan di meja

    // ── 2. PEMBAGIAN KARTU AWAL ──
    // *dealCard() mendereferensi pointer ke array deck agar nilai kartu disalin ke HandNode
    playerHand.addCard(*dealCard());
    dealerHand.addCard(*dealCard());
    playerHand.addCard(*dealCard());
    dealerHand.addCard(*dealCard());

    bool bust            = false;
    // Blackjack natural = total 21 tepat dari 2 kartu pertama (As + kartu bernilai 10)
    bool playerBlackjack = (playerHand.total() == 21 && playerHand.size == 2);
    bool dealerBJearly   = (dealerHand.total() == 21 && dealerHand.size == 2);

    showGameTable(p, playerHand, dealerHand, true, bet, "Kartu dibagikan! Giliran kamu.");

    // ── 3. GILIRAN PEMAIN ──
    if (!playerBlackjack) {
        char ch;
        do {
            printBorder();
            printBoxRow("  [H] Hit  = Minta kartu tambahan");
            printBoxRow("  [S] Stand = Berhenti, giliran dealer");
            printBorder();
            cout << "  Pilihan kamu (H/S): ";

            // Validasi input H/S — konsisten dengan printInlineError seperti input lainnya
            while (true) {
                cin >> ch;
                while (cin.get() != '\n') continue;
                if (ch == 'H' || ch == 'h' || ch == 'S' || ch == 's') break;
                printInlineError("Input tidak valid. Ketik 'H' untuk Hit atau 'S' untuk Stand.");
                cout << "  > ";
            }

            if (ch == 'H' || ch == 'h') {
                playerHand.addCard(*dealCard()); // Tambah kartu ke linked list tangan
                if (playerHand.total() > 21) {
                    // BUST = Kalah langsung
                    showGameTable(p, playerHand, dealerHand, true, bet,
                                  "BUST! Total kamu melebihi 21 -- KALAH!");
                    bust = true;
                    break;
                }
                showGameTable(p, playerHand, dealerHand, true, bet,
                              "Kartu ditambahkan. Total: " + to_string(playerHand.total()));
            } else { // 'S' atau 's'
                showGameTable(p, playerHand, dealerHand, true, bet, "Kamu STAND. Giliran dealer...");
            }
        } while (ch != 'S' && ch != 's' && !bust);
    } else {
        showGameTable(p, playerHand, dealerHand, true, bet, "BLACKJACK! Kartu sempurna!");
    }

    // ── 4. GILIRAN DEALER ──
    // Dealer hanya bermain jika pemain tidak bust
    if (!bust) {
        showGameTable(p, playerHand, dealerHand, false, bet, "Dealer membuka kartu tersembunyi...");
        // Aturan kasino: dealer WAJIB hit jika totalnya di bawah 17
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

    int  pt       = playerHand.total();
    int  dt       = dealerHand.total();
    bool dealerBJ = dealerBJearly; // dicatat sejak awal, sebelum dealer hit tambahan
    bool win      = false;
    bool push     = false;
    int  payout   = 0;

    if (bust) {
        p->losses++;
    } else if (playerBlackjack && !dealerBJ) {
        // Blackjack vs dealer biasa — bayaran 3:2
        payout = bet + bet * 3 / 2;
        p->chips += payout;
        p->wins++;
        win = true;
    } else if (playerBlackjack && dealerBJ) {
        // Sama-sama Blackjack — push
        p->chips += bet;
        push = true;
    } else if (dt > 21 || pt > dt) {
        // Dealer bust atau total pemain lebih tinggi
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
}

// ═══════════════════════════════════════════════════════
//  SECTION 5 — MENUS UTAMA & ALUR APP (Termasuk Fungsi Main)
//  TUGAS ANGGOTA 1: Mengatur Rute (Router) Program Secara Menyeluruh
// ═══════════════════════════════════════════════════════

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

void printMainMenu() {
    cout << "\n";
    printBorder();
    printBoxCenter("M E N U  U T A M A");
    printBorder();
    printBoxRow("  [ 1 ]  Kelola Pemain");
    printBorderThin();
    printBoxRow("  [ 2 ]  Mulai Bermain");
    printBorderThin();
    printBoxRow("  [ 0 ]  Keluar");
    printBorder();
    cout << "  Pilih menu: ";
}

void printPlayerManagementMenu() {
    cout << "\n";
    printBorder();
    printBoxCenter("K E L O L A   P E M A I N");
    printBorder();
    printBoxRow("  [ 1 ]  Lihat Semua Pemain");
    printBorderThin();
    printBoxRow("  [ 2 ]  Tambah Pemain Baru");
    printBorderThin();
    printBoxRow("  [ 3 ]  Edit Data Pemain");
    printBorderThin();
    printBoxRow("  [ 4 ]  Hapus Pemain");
    printBorderThin();
    printBoxRow("  [ 0 ]  Kembali ke Main Menu");
    printBorder();
    cout << "  Pilih menu: ";
}

void printSuccessMsg(const string& msg) {
    cout << "\n";
    printBorder();
    printBoxCenter("[ OK ] " + msg);
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

void printExitScreen(PlayerRegistry& reg) {
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

    if (reg.head) {
        cout << "\n";
        printBorder();
        printBoxCenter("RINGKASAN SESI INI");
        printBorder();
        printBoxRow("  Nama               Chips     Menang  Kalah");
        printBorderThin();
        PlayerProfile* cur = reg.head;
        while (cur) {
            string nama = cur->name;
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

void playerManagementMenu(PlayerRegistry& reg) {
    int choice;
    do {
        printPlayerManagementMenu();
        choice = getMenuInput(0, 4);

        switch (choice) {

            case 1: // READ — tampilkan semua pemain
                printInfoMsg("Menampilkan daftar semua pemain...");
                reg.showAll();
                break;

            case 2: { // CREATE — tambah pemain baru
                printInfoMsg("Tambah Pemain Baru");
                string name;
                int chips;
                cout << "  Nama pemain 1 kata (maks 15 karakter) : ";
                name = getNameInput();
                cout << "  Jumlah chips awal (1-999999)    : ";
                chips = getChipsInput();
                reg.addPlayer(name, chips);
                printSuccessMsg("Pemain \"" + name + "\" berhasil ditambahkan!");
                break;
            }

            case 3: { // UPDATE — edit data pemain
                printInfoMsg("Edit Data Pemain");
                reg.showAll();
                if (!reg.head) { printErrorMsg("Belum ada pemain yang terdaftar."); break; }
                int id;
                cout << "  Masukkan ID pemain yang ingin diedit: ";
                id = getIntInput();
                reg.updatePlayer(id);
                break;
            }

            case 4: { // DELETE — hapus pemain
                printInfoMsg("Hapus Pemain");
                reg.showAll();
                if (!reg.head) { printErrorMsg("Belum ada pemain yang terdaftar."); break; }
                int id;
                cout << "  Masukkan ID pemain yang ingin dihapus: ";
                id = getIntInput();

                PlayerProfile* target = reg.findById(id);
                if (target) {
                    cout << "  Hapus \"" << target->name << "\"? Tidak bisa dibatalkan! (y/n): ";
                    char c = getYesNoInput();
                    if (c == 'y' || c == 'Y') {
                        string dname = target->name;
                        reg.deletePlayer(id);
                        printSuccessMsg("Pemain \"" + dname + "\" berhasil dihapus.");
                    } else {
                        printInfoMsg("Penghapusan dibatalkan.");
                    }
                } else {
                    printErrorMsg("ID " + to_string(id) + " tidak ditemukan.");
                }
                break;
            }

            case 0:
                printInfoMsg("Kembali ke Main Menu...");
                break;

        }

    } while (choice != 0);
}

void playGameMenu(PlayerRegistry& reg) {
    if (!reg.head) {
        printErrorMsg("Belum ada pemain terdaftar. Tambah pemain dulu!");
        return;
    }

    cout << "\n";
    printBorder();
    printBoxCenter("PILIH PEMAIN");
    printBorder();
    printBoxRow("  Siapa yang akan bermain sesi ini?");
    printBorderThin();

    reg.showAll();

    int id;
    cout << "\n  Masukkan ID pemain: ";
    id = getIntInput();

    PlayerProfile* p = reg.findById(id);

    if (!p) {
        printErrorMsg("ID " + to_string(id) + " tidak ditemukan.");
        return;
    }

    if (p->chips <= 0) {
        printErrorMsg(p->name + " tidak punya chips. Edit pemain untuk menambah chips.");
        return;
    }

    cout << "\n";
    printBorder();
    printBoxCenter("Selamat datang, " + p->name + "!");
    printBorder();
    printBoxRow("  Chips saat ini : " + to_string(p->chips));
    printBoxRow("  Total menang   : " + to_string(p->wins));
    printBoxRow("  Total kalah    : " + to_string(p->losses));
    printBorderThin();
    printBoxRow("  Mau lihat aturan bermain dulu?");
    printBorder();
    cout << "  (y/n): ";
    char seeRules = getYesNoInput();
    if (seeRules == 'y' || seeRules == 'Y') showRules();

    char again = 'y';
    while ((again == 'y' || again == 'Y') && p->chips > 0) {
        playBlackjack(p);

        if (p->chips > 0) {
            cout << "\n";
            printBorder();
            printBoxCenter("Main lagi?");
            printBorder();
            printBoxRow("  Chips kamu sekarang: " + to_string(p->chips));
            printBorder();
            cout << "  (y/n): ";
            again = getYesNoInput();
        } else {
            printErrorMsg("Chips " + p->name + " habis! Pergi ke Kelola Pemain untuk tambah chips.");
        }
    }
}

#endif