#include "tampilan.h"

#include <algorithm> // memanggil fungsi untuk mencari, mengolah dan mengurutkan data
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

using namespace std;

namespace {
string input(const string& prompt) {
    cout << prompt;
    string hasil;
    getline(cin, hasil);
    return hasil;
}

bool bilangan(const string& teks, unsigned long long& hasil) {
    if (teks.empty()) return false;
    stringstream stream(teks);
    char sisa;
    return (stream >> hasil) && !(stream >> sisa);
}

unsigned long long inputRentang(const string& prompt,
                                unsigned long long minimum,
                                unsigned long long maksimum) {
    while (true) {
        unsigned long long nilai;
        if (bilangan(input(prompt), nilai) && nilai >= minimum && nilai <= maksimum) {
            return nilai;
        }
        cout << "Input harus berupa angka " << minimum << "-" << maksimum << ".\n";
    }
}

Tanggal inputTanggal() {
    Tanggal tanggal{};
    tanggal.hari = static_cast<uint8_t>(inputRentang("Hari (1-31): ", 1, 31));
    tanggal.bulan = static_cast<uint8_t>(inputRentang("Bulan (1-12): ", 1, 12));
    tanggal.tahun = static_cast<uint16_t>(inputRentang("Tahun (2000-2100): ", 2000, 2100));
    return tanggal;
}

void cetakSatu(const Barang& barang) {
    cout << left << setw(8) << barang.kode
              << setw(25) << barang.nama
              << setw(15) << barang.kategori
              << right << setw(12) << barang.harga
              << setw(7) << barang.stok << "  "
              << setfill('0') << setw(2) << static_cast<unsigned>(barang.tglMasuk.hari)
              << '/' << setw(2) << static_cast<unsigned>(barang.tglMasuk.bulan)
              << '/' << setw(4) << barang.tglMasuk.tahun << setfill(' ')
              << '\n';
}
}

void tampilkanMenu() {
    cout << "\n=== SISTEM INVENTARIS WARUNG MADURA ===\n"
              << "1. Tambah data\n2. Tampilkan seluruh data\n3. Cari data\n"
              << "4. Urutkan data\n5. Ubah data\n6. Hapus data\n"
              << "7. Statistik\n8. Info Memori\n0. Keluar\n";
}

void tampilkanData(const Barang data[], size_t jumlah) {
    if (jumlah == 0) {
        cout << "Belum ada data.\n";
        return;
    }
    cout << left << setw(8) << "Kode" << setw(25) << "Nama"
              << setw(15) << "Kategori" << right << setw(12) << "Harga"
              << setw(7) << "Stok" << "  Tanggal\n";
    cout << string(88, '-') << '\n';
    for (size_t i = 0; i < jumlah; ++i) cetakSatu(data[i]);
    cout << "Total: " << jumlah << " data\n";
}

void tambahData(Barang data[], size_t& jumlah) {
    if (jumlah >= KAPASITAS_DATA) {
        cout << "Kapasitas data penuh.\n";
        return;
    }
    Barang barang{};
    while (true) {
        string kode = input("Kode (BRG...): ");
        if (kodeValid(kode) && !kodeSudahAda(data, jumlah, kode)) {
            copy(kode.begin(), kode.end(), barang.kode);
            break;
        }
        cout << "Kode tidak valid atau sudah digunakan.\n";
    }
    while (true) {
        string nama = input("Nama barang: ");
        if (!nama.empty() && nama.size() < sizeof(barang.nama)) {
            copy(nama.begin(), nama.end(), barang.nama);
            break;
        }
        cout << "Nama wajib diisi (maksimal 29 karakter).\n";
    }
    while (true) {
        string kategori = input("Kategori: ");
        if (!kategori.empty() && kategori.size() < sizeof(barang.kategori)) {
            copy(kategori.begin(), kategori.end(), barang.kategori);
            break;
        }
        cout << "Kategori wajib diisi (maksimal 14 karakter).\n";
    }
    barang.harga = static_cast<uint32_t>(inputRentang("Harga (0-100000000): ", 0, 100000000));
    barang.stok = static_cast<uint16_t>(inputRentang("Stok (0-10000): ", 0, 10000));
    barang.tglMasuk = inputTanggal();
    ++jumlah;
    data[jumlah - 1] = barang;
    cout << "Data berhasil ditambahkan.\n";
}

void cariData(const Barang data[], size_t jumlah) {
    string kunci = input("Masukkan kata kunci nama/kategori/kode: ");
    bool ditemukan = false;
    for (size_t i = 0; i < jumlah; ++i) {
        if (mengandungTanpaKapital(data[i].kode, kunci) ||
            mengandungTanpaKapital(data[i].nama, kunci) ||
            mengandungTanpaKapital(data[i].kategori, kunci)) {
            if (!ditemukan) {
                cout << "Hasil pencarian:\n";
                ditemukan = true;
            }
            cetakSatu(data[i]);
        }
    }
    if (!ditemukan) cout << "Data tidak ditemukan.\n";
}

void ubahData(Barang data[], size_t jumlah) {
    string kode = input("Kode barang yang akan diubah: ");
    int indeks = cariDenganKode(data, jumlah, kode);
    if (indeks < 0) {
        cout << "Kode tidak ditemukan.\n";
        return;
    }
    Barang& barang = data[indeks];
    string nilai = input("Nama baru (kosong = tetap): ");
    if (!nilai.empty() && nilai.size() < sizeof(barang.nama)) {
        fill(begin(barang.nama), end(barang.nama), '\0');
        copy(nilai.begin(), nilai.end(), barang.nama);
    }
    nilai = input("Kategori baru (kosong = tetap): ");
    if (!nilai.empty() && nilai.size() < sizeof(barang.kategori)) {
        fill(begin(barang.kategori), end(barang.kategori), '\0');
        copy(nilai.begin(), nilai.end(), barang.kategori);
    }
    nilai = input("Harga baru (kosong = tetap): ");
    unsigned long long angkaBaru;
    if (!nilai.empty() && bilangan(nilai, angkaBaru) && angkaBaru <= 100000000)
        barang.harga = static_cast<uint32_t>(angkaBaru);
    nilai = input("Stok baru (kosong = tetap): ");
    if (!nilai.empty() && bilangan(nilai, angkaBaru) && angkaBaru <= 10000)
        barang.stok = static_cast<uint16_t>(angkaBaru);
    if (konfirmasi("Ubah tanggal masuk? (y/n): ")) barang.tglMasuk = inputTanggal();
    cout << "Data berhasil diubah.\n";
}

void hapusData(Barang data[], size_t& jumlah) {
    string kode = input("Kode barang yang akan dihapus: ");
    int indeks = cariDenganKode(data, jumlah, kode);
    if (indeks < 0) {
        cout << "Kode tidak ditemukan.\n";
        return;
    }
    if (konfirmasi("Yakin menghapus data? (y/n): ")) {
        hapusBarang(data, jumlah, static_cast<size_t>(indeks));
        cout << "Data berhasil dihapus.\n";
    }
}

void tampilkanStatistik(const Barang data[], size_t jumlah) {
    if (jumlah == 0) {
        cout << "Belum ada data.\n";
        return;
    }
    unsigned long long totalNilai = 0, totalHarga = 0;
    size_t indeksHargaMax = 0, indeksStokMin = 0;
    for (size_t i = 0; i < jumlah; ++i) {
        totalNilai += static_cast<unsigned long long>(data[i].harga) * data[i].stok;
        totalHarga += data[i].harga;
        if (data[i].harga > data[indeksHargaMax].harga) indeksHargaMax = i;
        if (data[i].stok < data[indeksStokMin].stok) indeksStokMin = i;
    }
    cout << "Jumlah jenis barang : " << jumlah << '\n'
              << "Total nilai stok   : Rp" << totalNilai << '\n'
              << "Rata-rata harga    : Rp" << totalHarga / jumlah << '\n'
              << "Harga tertinggi    : " << data[indeksHargaMax].nama
              << " (Rp" << data[indeksHargaMax].harga << ")\n"
              << "Stok terendah      : " << data[indeksStokMin].nama
              << " (" << data[indeksStokMin].stok << ")\n";
}

void tampilkanInfoMemori(const Barang data[]) {
    cout << "sizeof(Tanggal)       : " << sizeof(Tanggal) << " byte\n"
              << "sizeof(Barang)        : " << sizeof(Barang) << " byte\n"
              << "Kapasitas array       : " << KAPASITAS_DATA << " record\n"
              << "sizeof(array data)    : " << sizeof(Barang) * KAPASITAS_DATA << " byte\n"
              << "Memori data terpakai  : " << sizeof(Barang) * KAPASITAS_DATA
              << " byte (array tetap)\n";
    (void)data;
}

bool konfirmasi(const string& pertanyaan) {
    string jawaban = input(pertanyaan);
    return !jawaban.empty() &&
           (jawaban[0] == 'y' || jawaban[0] == 'Y');
}
