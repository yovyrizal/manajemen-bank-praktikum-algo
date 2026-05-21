#include <iostream>
#include <string>
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
        case 1: inputNasabah() break;
        case 2: tampilkanNasabah() break;
        case 3: editSaldo() break;
        case 4: sortingMenu() break;
        case 5: cariNasabah() break;
        case 6: hapusData() break;
        case 7: cout << "Terima kasih, sampai jumpa!\n"; break;
        }

    } while (pilihanMenu != 7);
}

// ============= MAIN SECTION ===============
int main()
{
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
