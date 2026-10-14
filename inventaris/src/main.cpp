#include "berkas.h"
#include "tampilan.h"

#include <iostream>
#include <string>

int main() {
    Barang data[KAPASITAS_DATA]{};
    std::size_t jumlah = bacaData("data.txt", data, KAPASITAS_DATA);
    std::cout << "Data valid yang dimuat: " << jumlah << '\n';

    bool berjalan = true;
    while (berjalan) {
        tampilkanMenu();
        std::string pilihan;
        std::cout << "Pilihan: ";
        if (!std::getline(std::cin, pilihan)) {
            std::cout << "\nInput berakhir. Data disimpan.\n";
            simpanData("data.txt", data, jumlah);
            break;
        }
        if (pilihan == "1") tambahData(data, jumlah);
        else if (pilihan == "2") tampilkanData(data, jumlah);
        else if (pilihan == "3") cariData(data, jumlah);
        else if (pilihan == "4") {
            std::cout << "1. Harga\n2. Stok\n3. Nama\n";
            std::getline(std::cin, pilihan);
            if (pilihan == "1" || pilihan == "2" || pilihan == "3") {
                urutkanBarang(data, jumlah, std::stoi(pilihan));
                std::cout << "Data berhasil diurutkan naik.\n";
            } else {
                std::cout << "Pilihan urutan tidak valid.\n";
            }
        } else if (pilihan == "5") ubahData(data, jumlah);
        else if (pilihan == "6") hapusData(data, jumlah);
        else if (pilihan == "7") tampilkanStatistik(data, jumlah);
        else if (pilihan == "8") tampilkanInfoMemori(data);
        else if (pilihan == "0") {
            if (simpanData("data.txt", data, jumlah)) std::cout << "Data tersimpan. Program selesai.\n";
            berjalan = false;
        } else {
            std::cout << "Pilihan tidak valid.\n";
        }
    }
    return 0;
}
