#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <ctime>
using namespace std;

struct Node
{
    string card;
    string nama;
    float saldo;
    Node *kanan;
    Node *kiri;
};

Node *head = nullptr;

// ================= BUAT NODE
struct Node *buatNode(string card, string nama, float saldo)
{
    Node *newNode = new Node;
    newNode->card  = card;
    newNode->nama  = nama;
    newNode->saldo = saldo;
    newNode->kanan = nullptr;
    newNode->kiri  = nullptr;
    return newNode;
}

// ================= GENERATE CARD NUMBER
bool cardSudahAda(string card)
{
    Node *temp = head;
    while (temp != nullptr)
    {
        if (temp->card == card)
            return true;
        temp = temp->kanan;
    }
    return false;
}

string generateCardNumber()
{
    static int counter = 0;
    string card = "";

    do {
        srand(time(0) + counter++);
        card = "";
        for (int i = 0; i < 16; i++)
        {
            card += to_string(rand() % 10);
        }
    } while (cardSudahAda(card));

    return card;
}

// ================= FILE SECTION
void simpanKeFile()
{
    FILE *file = fopen("dataNasabah.csv", "w");
    if (!file)
    {
        cout << "Gagal membuka file untuk disimpan!\n";
        return;
    }
    Node *temp = head;
    while (temp != nullptr)
    {
        fprintf(file, "%s,%s,%.2f\n", temp->card.c_str(), temp->nama.c_str(), temp->saldo);
        temp = temp->kanan;
    }
    fclose(file);
}

void muatDariFile()
{
    FILE *file = fopen("dataNasabah.csv", "r");
    if (!file)
        return;

    Node *curr = head;
    while (curr != nullptr)
    {
        Node *next = curr->kanan;
        delete curr;
        curr = next;
    }
    head = nullptr;

    char card[17];
    char nama[100];
    float saldo;
    while (fscanf(file, "%16[^,],%99[^,],%f\n", card, nama, &saldo) == 3)
    {
        Node *newNode = buatNode(string(card), string(nama), saldo);
        if (head == nullptr)
        {
            head = newNode;
        }
        else
        {
            Node *temp = head;
            while (temp->kanan != nullptr)
            {
                temp = temp->kanan;
            }
            temp->kanan = newNode;
            newNode->kiri = temp;
        }
    }
    fclose(file);
}

// ============================= INPUT SECTION
void inputNasabah()
{
    string nama;
    float saldo;

    string card = generateCardNumber();

    cout << "\n=== INPUT NASABAH BARU ===\n";
    cout << "Masukkan Nama Nasabah : ";
    cin.ignore();
    getline(cin, nama);
    cout << "Masukkan Saldo Awal   : ";
    cin >> saldo;

    Node *newNode = buatNode(card, nama, saldo);

    if (head == nullptr)
    {
        head = newNode;
    }
    else
    {
        Node *temp = head;
        while (temp->kanan != nullptr)
        {
            temp = temp->kanan;
        }
        temp->kanan = newNode;
        newNode->kiri = temp;
    }

    simpanKeFile();
    cout << "Nasabah \"" << nama << "\" berhasil ditambahkan!\n";
    cout << "Card Number           : " << card << "\n";
}

// ===================== DISPLAY SECTION
void tampilkanNasabah()
{
    cout << "\n=== DAFTAR NASABAH ===\n";
    if (head == nullptr)
    {
        cout << "Belum ada data nasabah.\n";
        return;
    }

    printf("%-18s %-25s %15s\n", "Card Number", "Nama", "Saldo");
    cout << string(60, '-') << "\n";

    Node *temp = head;
    while (temp != nullptr)
    {
        printf("%-18s %-25s %15.2f\n", temp->card.c_str(), temp->nama.c_str(), temp->saldo);
        temp = temp->kanan;
    }
    cout << string(60, '-') << "\n";
}

// ============== HELPER: CARI NODE BY CARD
Node *cariNodeByCard(string card)
{
    Node *temp = head;
    while (temp != nullptr)
    {
        if (temp->card == card)
            return temp;
        temp = temp->kanan;
    }
    return nullptr;
}

// ============== EDIT SALDO SECTION
void tambahSaldo()
{
    string card;
    float jumlah;

    cout << "\n=== TAMBAH SALDO ===\n";
    cout << "Masukkan Card Number  : ";
    cin.ignore();
    getline(cin, card);

    Node *target = cariNodeByCard(card);
    if (target == nullptr)
    {
        cout << "Nasabah dengan card number \"" << card << "\" tidak ditemukan!\n";
        return;
    }

    cout << "Nama                  : " << target->nama << "\n";
    cout << "Saldo saat ini        : " << target->saldo << "\n";
    cout << "Masukkan Jumlah Tambah: ";
    cin >> jumlah;

    if (jumlah <= 0)
    {
        cout << "Jumlah harus lebih dari 0!\n";
        return;
    }

    target->saldo += jumlah;
    simpanKeFile();
    cout << "Saldo berhasil ditambah. Saldo baru: " << target->saldo << "\n";
}

void tarikSaldo()
{
    string card;
    float jumlah;

    cout << "\n=== TARIK SALDO ===\n";
    cout << "Masukkan Card Number  : ";
    cin.ignore();
    getline(cin, card);

    Node *target = cariNodeByCard(card);
    if (target == nullptr)
    {
        cout << "Nasabah dengan card number \"" << card << "\" tidak ditemukan!\n";
        return;
    }

    cout << "Nama                  : " << target->nama << "\n";
    cout << "Saldo saat ini        : " << target->saldo << "\n";
    cout << "Masukkan Jumlah Tarik : ";
    cin >> jumlah;

    if (jumlah <= 0)
    {
        cout << "Jumlah harus lebih dari 0!\n";
        return;
    }
    if (jumlah > target->saldo)
    {
        cout << "Saldo tidak cukup! Saldo saat ini: " << target->saldo << "\n";
        return;
    }

    target->saldo -= jumlah;
    simpanKeFile();
    cout << "Penarikan berhasil. Saldo baru: " << target->saldo << "\n";
}

void ubahNominal()
{
    string card;
    float nominalBaru;

    cout << "\n=== UBAH NOMINAL SALDO ===\n";
    cout << "Masukkan Card Number    : ";
    cin.ignore();
    getline(cin, card);

    Node *target = cariNodeByCard(card);
    if (target == nullptr)
    {
        cout << "Nasabah dengan card number \"" << card << "\" tidak ditemukan!\n";
        return;
    }

    cout << "Nama                    : " << target->nama << "\n";
    cout << "Saldo saat ini          : " << target->saldo << "\n";
    cout << "Masukkan Nominal Baru   : ";
    cin >> nominalBaru;

    if (nominalBaru < 0)
    {
        cout << "Nominal tidak boleh negatif!\n";
        return;
    }

    target->saldo = nominalBaru;
    simpanKeFile();
    cout << "Saldo berhasil diubah menjadi: " << target->saldo << "\n";
}

void editSaldo()
{
    int menuEdit;

    cout << "\n======= EDIT SALDO ======\n";
    cout << "1. Tambah Saldo\n";
    cout << "2. Tarik Saldo\n";
    cout << "3. Ubah Nominal\n";
    cout << "=== Pilih Menu (1/2/3) : ";
    cin >> menuEdit;

    switch (menuEdit)
    {
    case 1:
        tambahSaldo();
        break;
    case 2:
        tarikSaldo();
        break;
    case 3:
        ubahNominal();
        break;
    default:
        cout << "Pilihan tidak valid!\n";
        break;
    }
}

// ===================== SORTING SECTION
// Bubble Sort ascending berdasarkan nama
void bubbleSortNama()
{
    if (head == nullptr)
        return;

    bool swapped;
    do
    {
        swapped = false;
        Node *curr = head;
        while (curr->kanan != nullptr)
        {
            if (curr->nama > curr->kanan->nama)
            {
                string tempCard      = curr->card;
                curr->card           = curr->kanan->card;
                curr->kanan->card    = tempCard;

                string tempNama      = curr->nama;
                curr->nama           = curr->kanan->nama;
                curr->kanan->nama    = tempNama;

                float tempSaldo      = curr->saldo;
                curr->saldo          = curr->kanan->saldo;
                curr->kanan->saldo   = tempSaldo;

                swapped = true;
            }
            curr = curr->kanan;
        }
    } while (swapped);
}

// Quick Sort descending berdasarkan saldo
Node *getTail(Node *curr)
{
    while (curr != nullptr && curr->kanan != nullptr)
        curr = curr->kanan;
    return curr;
}

void quickSortSaldo(Node *low, Node *high)
{
    if (high != nullptr && low != high && low != high->kanan)
    {
        float pivotSaldo = high->saldo;

        Node *i = low->kiri;

        for (Node *j = low; j != high; j = j->kanan)
        {
            if (j->saldo >= pivotSaldo)
            {
                i = (i == nullptr) ? low : i->kanan;

                string tempCard      = i->card;
                i->card              = j->card;
                j->card              = tempCard;

                string tempNama      = i->nama;
                i->nama              = j->nama;
                j->nama              = tempNama;

                float tempSaldo      = i->saldo;
                i->saldo             = j->saldo;
                j->saldo             = tempSaldo;
            }
        }

        i = (i == nullptr) ? low : i->kanan;

        string tempCard      = i->card;
        i->card              = high->card;
        high->card           = tempCard;

        string tempNama      = i->nama;
        i->nama              = high->nama;
        high->nama           = tempNama;

        float tempSaldo      = i->saldo;
        i->saldo             = high->saldo;
        high->saldo          = tempSaldo;

        quickSortSaldo(low, i->kiri);
        quickSortSaldo(i->kanan, high);
    }
}

void sortSaldo()
{
    Node *tail = getTail(head);
    quickSortSaldo(head, tail);
}

void sortingMenu()
{
    int menu;

    cout << "\n======= MENU SORTING ======\n";
    cout << "1. Berdasarkan Nama  (Bubble Sort Ascending)\n";
    cout << "2. Berdasarkan Saldo (Quick Sort Descending)\n";
    cout << "=== Pilih Menu (1/2) : ";
    cin >> menu;

    if (head == nullptr)
    {
        cout << "Belum ada data nasabah.\n";
        return;
    }

    switch (menu)
    {
    case 1:
        bubbleSortNama();
        simpanKeFile();
        cout << "Data berhasil diurutkan berdasarkan nama (A-Z).\n";
        tampilkanNasabah();
        break;
    case 2:
        sortSaldo();
        simpanKeFile();
        cout << "Data berhasil diurutkan berdasarkan saldo (terbesar ke terkecil).\n";
        tampilkanNasabah();
        break;
    default:
        cout << "Pilihan tidak valid!\n";
        break;
    }
}

// =============== SEARCHING SECTION

// Fungsi mengubah string menjadi huruf kecil semua
string toLower(string str)
{
    for (int i = 0; i < (int)str.length(); i++)
    {
        str[i] = tolower(str[i]);
    }
    return str;
}

void searchingMenu()
{
    string cariNasabah;

    cout << "\n======= MENU SEARCHING ======\n";
    cout << "=== Masukkan nama nasabah yang ingin dicari : ";
    cin.ignore();
    getline(cin, cariNasabah);

    Node *temp = head;
    bool ditemukan = false;

    while (temp != nullptr)
    {
        if (toLower(temp->nama) == toLower(cariNasabah))
        {
            if (!ditemukan)
            {
                cout << "\nHasil Pencarian:\n";
                printf("%-18s %-25s %15s\n", "Card Number", "Nama", "Saldo");
                cout << string(60, '-') << "\n";
            }
            printf("%-18s %-25s %15.2f\n", temp->card.c_str(), temp->nama.c_str(), temp->saldo);
            ditemukan = true;
        }
        temp = temp->kanan;
    }

    if (!ditemukan)
    {
        cout << "Nasabah \"" << cariNasabah << "\" tidak ditemukan!\n";
    }
}

// =========== DELETE SECTION
void hapusData()
{
    string hapusCard;

    cout << "\n======= MENU HAPUS ======\n";
    cout << "=== Masukkan card number nasabah yang ingin dihapus : ";
    cin.ignore();
    getline(cin, hapusCard);

    Node *temp = cariNodeByCard(hapusCard);

    if (temp == nullptr)
    {
        cout << "Nasabah dengan card number \"" << hapusCard << "\" tidak ditemukan!\n";
        return;
    }

    // Tampilkan data nasabah yang akan dihapus
    cout << "\nData nasabah yang akan dihapus:\n";
    printf("%-18s %-25s %15s\n", "Card Number", "Nama", "Saldo");
    cout << string(60, '-') << "\n";
    printf("%-18s %-25s %15.2f\n", temp->card.c_str(), temp->nama.c_str(), temp->saldo);
    cout << string(60, '-') << "\n";

    // Konfirmasi hapus
    char konfirmasi;
    cout << "Apakah Anda yakin ingin menghapus data ini? (y/n) : ";
    cin >> konfirmasi;

    if (konfirmasi != 'y' && konfirmasi != 'Y')
    {
        cout << "Penghapusan dibatalkan.\n";
        return;
    }

    // Proses hapus
    if (temp->kiri != nullptr)
    {
        temp->kiri->kanan = temp->kanan;
    }
    else
    {
        head = temp->kanan;
    }

    if (temp->kanan != nullptr)
    {
        temp->kanan->kiri = temp->kiri;
    }

    string namaHapus = temp->nama;
    delete temp;

    simpanKeFile();
    cout << "Nasabah \"" << namaHapus << "\" berhasil dihapus.\n";
}

// ======================== MENU SECTION
void menu()
{
    int pilihanMenu;

    do
    {
        cout << "\n======= MENU ADMIN ======\n";
        cout << "1. Input Nasabah\n";
        cout << "2. Tampilkan Nasabah\n";
        cout << "3. Edit Saldo\n";
        cout << "4. Sorting\n";
        cout << "5. Cari Nasabah (Searching)\n";
        cout << "6. Hapus Data Nasabah\n";
        cout << "7. Keluar\n";

        do
        {
            cout << "=== Masukkan menu yang ingin dipilih (1-7) : ";
            cin >> pilihanMenu;
        } while (pilihanMenu < 1 || pilihanMenu > 7);

        switch (pilihanMenu)
        {
        case 1:
            inputNasabah();
            break;
        case 2:
            tampilkanNasabah();
            break;
        case 3:
            editSaldo();
            break;
        case 4:
            sortingMenu();
            break;
        case 5:
            searchingMenu();
            break;
        case 6:
            hapusData();
            break;
        case 7:
            cout << "Terima kasih, sampai jumpa!\n";
            break;
        }

    } while (pilihanMenu != 7);
}

// ============= MAIN SECTION ===============
int main()
{
    muatDariFile();

    string adminUsername = "admin";
    string adminPassword = "admin123";

    string username, password;
    int percobaan = 3;

    cout << "=== ADMIN LOGIN PAGE ===\n";
    do
    {
        cout << "Masukkan Username : ";
        cin >> username;
        cout << "Masukkan Password : ";
        cin >> password;

        if (username == adminUsername && password == adminPassword)
        {
            cout << "Login berhasil! Selamat datang, " << username << ".\n";
            menu();
            return 0;
        }
        else
        {
            percobaan--;
            if (percobaan > 0)
                cout << "Username atau Password salah! Sisa " << percobaan << " percobaan.\n";
            else
                cout << "Akses ditolak! Terlalu banyak percobaan login.\n";
        }

    } while (percobaan > 0);

    return 0;
}