#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
using namespace std;

struct Node
{
    int id;
    string nama;
    float saldo;
    Node *kanan;
    Node *kiri;
};

Node *head = nullptr;

// ================= BUAT NODE
struct Node *buatNode(int id, string nama, float saldo)
{
    Node *newNode = new Node;
    newNode->id = id;
    newNode->nama = nama;
    newNode->saldo = saldo;
    newNode->kanan = nullptr;
    newNode->kiri = nullptr;
    return newNode;
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
        fprintf(file, "%d,%s,%.2f\n", temp->id, temp->nama.c_str(), temp->saldo);
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

    int id;
    char nama[100];
    float saldo;
    while (fscanf(file, "%d,%99[^,],%f\n", &id, nama, &saldo) == 3)
    {
        Node *newNode = buatNode(id, string(nama), saldo);
        if (head == nullptr)
        {
            head = newNode;
        }
        else
        {
            Node *temp = head;
            while (temp->kanan != nullptr)
                temp = temp->kanan;
            temp->kanan = newNode;
            newNode->kiri = temp;
        }
    }
    fclose(file);
}

// ============================= INPUT SECTION
int cariIdTerbesar()
{
    int maxId = 0;
    Node *temp = head;
    while (temp != nullptr) {
        if (temp->id > maxId) maxId = temp->id;
        temp = temp->kanan;
    }
    return maxId;
}

void inputNasabah()
{
    string nama;
    float saldo;

    int id = cariIdTerbesar() + 1;

    cout << "\n=== INPUT NASABAH BARU ===\n";
    cout << "Masukkan Nama Nasabah : ";
    cin.ignore();
    getline(cin, nama);
    cout << "Masukkan Saldo Awal   : ";
    cin >> saldo;

    Node *newNode = buatNode(id, nama, saldo);

    if (head == nullptr)
    {
        head = newNode;
    }
    else
    {
        Node *temp = head;
        while (temp->kanan != nullptr)
            temp = temp->kanan;
        temp->kanan = newNode;
        newNode->kiri = temp;
    }

    simpanKeFile();
    cout << "Nasabah \"" << nama << "\" berhasil ditambahkan! (ID: " << id << ")\n";
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

    cout << left;
    printf("%-5s %-25s %15s\n", "ID", "Nama", "Saldo");
    cout << string(47, '-') << "\n";

    Node *temp = head;
    while (temp != nullptr)
    {
        printf("%-5d %-25s %15.2f\n", temp->id, temp->nama.c_str(), temp->saldo);
        temp = temp->kanan;
    }
    cout << string(47, '-') << "\n";
}

// ============== EDIT SALDO SECTION
Node *cariNodeByNama(string nama)
{
    Node *temp = head;
    while (temp != nullptr)
    {
        if (temp->nama == nama)
            return temp;
        temp = temp->kanan;
    }
    return nullptr;
}

void tambahSaldo()
{
    string nama;
    float jumlah;

    cout << "\n=== TAMBAH SALDO ===\n";
    cout << "Masukkan Nama Nasabah : ";
    cin.ignore();
    getline(cin, nama);

    Node *target = cariNodeByNama(nama);
    if (target == nullptr)
    {
        cout << "Nasabah \"" << nama << "\" tidak ditemukan!\n";
        return;
    }

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
    string nama;
    float jumlah;

    cout << "\n=== TARIK SALDO ===\n";
    cout << "Masukkan Nama Nasabah : ";
    cin.ignore();
    getline(cin, nama);

    Node *target = cariNodeByNama(nama);
    if (target == nullptr)
    {
        cout << "Nasabah \"" << nama << "\" tidak ditemukan!\n";
        return;
    }

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
    string nama;
    float nominalBaru;

    cout << "\n=== UBAH NOMINAL SALDO ===\n";
    cout << "Masukkan Nama Nasabah   : ";
    cin.ignore();
    getline(cin, nama);

    Node *target = cariNodeByNama(nama);
    if (target == nullptr)
    {
        cout << "Nasabah \"" << nama << "\" tidak ditemukan!\n";
        return;
    }

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
    case 1: tambahSaldo(); break;
    case 2: tarikSaldo();  break;
    case 3: ubahNominal(); break;
    default: cout << "Pilihan tidak valid!\n"; break;
    }
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
        case 1: inputNasabah();    break;
        case 2: tampilkanNasabah(); break;
        case 3: editSaldo();       break;
        case 4: cout << "Fitur belum tersedia\n"; break;
        case 5: cout << "Fitur belum tersedia\n"; break;
        case 6: cout << "Fitur belum tersedia\n"; break;
        case 7: cout << "Terima kasih, sampai jumpa!\n"; break;
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