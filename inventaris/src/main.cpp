#include "berkas.h"
#include "tampilan.h"

#include <iostream>
#include <string>

using namespace std;

int main() {
    constexpr const char* NAMA_BERKAS_DATA = "data/data.txt";
    Barang data[KAPASITAS_DATA]{};
    size_t jumlah = bacaData(NAMA_BERKAS_DATA, data, KAPASITAS_DATA);
    cout << "Data valid yang dimuat: " << jumlah << '\n';

    bool berjalan = true;
    while (berjalan) {
        tampilkanMenu();
        string pilihan;
        cout << "Pilihan: ";
        if (!getline(cin, pilihan)) {
            cout << "\nInput berakhir. Data disimpan.\n";
            simpanData(NAMA_BERKAS_DATA, data, jumlah);
            break;
        }
        if (pilihan == "1") tambahData(data, jumlah);
        else if (pilihan == "2") tampilkanData(data, jumlah);
        else if (pilihan == "3") cariData(data, jumlah);
        else if (pilihan == "4") {
            cout << "1. Harga\n2. Stok\n3. Nama\n";
            getline(cin, pilihan);
            if (pilihan == "1" || pilihan == "2" || pilihan == "3") {
                urutkanBarang(data, jumlah, stoi(pilihan));
                cout << "Data berhasil diurutkan naik.\n";
            } else {
                cout << "Pilihan urutan tidak valid.\n";
            }
        } else if (pilihan == "5") ubahData(data, jumlah);
        else if (pilihan == "6") hapusData(data, jumlah);
        else if (pilihan == "7") tampilkanStatistik(data, jumlah);
        else if (pilihan == "8") tampilkanInfoMemori(data);
        else if (pilihan == "0") {
            if (simpanData(NAMA_BERKAS_DATA, data, jumlah)) cout << "Data tersimpan. Program selesai.\n";
            berjalan = false;
        } else {
            cout << "Pilihan tidak valid.\n";
        }
    }
    return 0;
}
