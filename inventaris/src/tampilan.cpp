#include "tampilan.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

namespace {
std::string input(const std::string& prompt) {
    std::cout << prompt;
    std::string hasil;
    std::getline(std::cin, hasil);
    return hasil;
}

bool bilangan(const std::string& teks, unsigned long long& hasil) {
    if (teks.empty()) return false;
    std::stringstream stream(teks);
    char sisa;
    return (stream >> hasil) && !(stream >> sisa);
}

unsigned long long inputRentang(const std::string& prompt,
                                unsigned long long minimum,
                                unsigned long long maksimum) {
    while (true) {
        unsigned long long nilai;
        if (bilangan(input(prompt), nilai) && nilai >= minimum && nilai <= maksimum) {
            return nilai;
        }
        std::cout << "Input harus berupa angka " << minimum << "-" << maksimum << ".\n";
    }
}

Tanggal inputTanggal() {
    Tanggal tanggal{};
    tanggal.hari = static_cast<std::uint8_t>(inputRentang("Hari (1-31): ", 1, 31));
    tanggal.bulan = static_cast<std::uint8_t>(inputRentang("Bulan (1-12): ", 1, 12));
    tanggal.tahun = static_cast<std::uint16_t>(inputRentang("Tahun (2000-2100): ", 2000, 2100));
    return tanggal;
}

void cetakSatu(const Barang& barang) {
    std::cout << std::left << std::setw(8) << barang.kode
              << std::setw(25) << barang.nama
              << std::setw(15) << barang.kategori
              << std::right << std::setw(12) << barang.harga
              << std::setw(7) << barang.stok << "  "
              << std::setfill('0') << std::setw(2) << static_cast<unsigned>(barang.tglMasuk.hari)
              << '/' << std::setw(2) << static_cast<unsigned>(barang.tglMasuk.bulan)
              << '/' << std::setw(4) << barang.tglMasuk.tahun << std::setfill(' ')
              << '\n';
}
}

void tampilkanMenu() {
    std::cout << "\n=== SISTEM INVENTARIS WARUNG MADURA ===\n"
              << "1. Tambah data\n2. Tampilkan seluruh data\n3. Cari data\n"
              << "4. Urutkan data\n5. Ubah data\n6. Hapus data\n"
              << "7. Statistik\n8. Info Memori\n0. Keluar\n";
}

void tampilkanData(const Barang data[], std::size_t jumlah) {
    if (jumlah == 0) {
        std::cout << "Belum ada data.\n";
        return;
    }
    std::cout << std::left << std::setw(8) << "Kode" << std::setw(25) << "Nama"
              << std::setw(15) << "Kategori" << std::right << std::setw(12) << "Harga"
              << std::setw(7) << "Stok" << "  Tanggal\n";
    std::cout << std::string(88, '-') << '\n';
    for (std::size_t i = 0; i < jumlah; ++i) cetakSatu(data[i]);
    std::cout << "Total: " << jumlah << " data\n";
}

void tambahData(Barang data[], std::size_t& jumlah) {
    if (jumlah >= KAPASITAS_DATA) {
        std::cout << "Kapasitas data penuh.\n";
        return;
    }
    Barang barang{};
    while (true) {
        std::string kode = input("Kode (BRG...): ");
        if (kodeValid(kode) && !kodeSudahAda(data, jumlah, kode)) {
            std::copy(kode.begin(), kode.end(), barang.kode);
            break;
        }
        std::cout << "Kode tidak valid atau sudah digunakan.\n";
    }
    while (true) {
        std::string nama = input("Nama barang: ");
        if (!nama.empty() && nama.size() < sizeof(barang.nama)) {
            std::copy(nama.begin(), nama.end(), barang.nama);
            break;
        }
        std::cout << "Nama wajib diisi (maksimal 29 karakter).\n";
    }
    while (true) {
        std::string kategori = input("Kategori: ");
        if (!kategori.empty() && kategori.size() < sizeof(barang.kategori)) {
            std::copy(kategori.begin(), kategori.end(), barang.kategori);
            break;
        }
        std::cout << "Kategori wajib diisi (maksimal 14 karakter).\n";
    }
    barang.harga = static_cast<std::uint32_t>(inputRentang("Harga (0-100000000): ", 0, 100000000));
    barang.stok = static_cast<std::uint16_t>(inputRentang("Stok (0-10000): ", 0, 10000));
    barang.tglMasuk = inputTanggal();
    ++jumlah;
    data[jumlah - 1] = barang;
    std::cout << "Data berhasil ditambahkan.\n";
}

void cariData(const Barang data[], std::size_t jumlah) {
    std::string kunci = input("Masukkan kata kunci nama/kategori/kode: ");
    bool ditemukan = false;
    for (std::size_t i = 0; i < jumlah; ++i) {
        if (mengandungTanpaKapital(data[i].kode, kunci) ||
            mengandungTanpaKapital(data[i].nama, kunci) ||
            mengandungTanpaKapital(data[i].kategori, kunci)) {
            if (!ditemukan) {
                std::cout << "Hasil pencarian:\n";
                ditemukan = true;
            }
            cetakSatu(data[i]);
        }
    }
    if (!ditemukan) std::cout << "Data tidak ditemukan.\n";
}

void ubahData(Barang data[], std::size_t jumlah) {
    std::string kode = input("Kode barang yang akan diubah: ");
    int indeks = cariDenganKode(data, jumlah, kode);
    if (indeks < 0) {
        std::cout << "Kode tidak ditemukan.\n";
        return;
    }
    Barang& barang = data[indeks];
    std::string nilai = input("Nama baru (kosong = tetap): ");
    if (!nilai.empty() && nilai.size() < sizeof(barang.nama)) {
        std::fill(std::begin(barang.nama), std::end(barang.nama), '\0');
        std::copy(nilai.begin(), nilai.end(), barang.nama);
    }
    nilai = input("Kategori baru (kosong = tetap): ");
    if (!nilai.empty() && nilai.size() < sizeof(barang.kategori)) {
        std::fill(std::begin(barang.kategori), std::end(barang.kategori), '\0');
        std::copy(nilai.begin(), nilai.end(), barang.kategori);
    }
    nilai = input("Harga baru (kosong = tetap): ");
    unsigned long long angkaBaru;
    if (!nilai.empty() && bilangan(nilai, angkaBaru) && angkaBaru <= 100000000)
        barang.harga = static_cast<std::uint32_t>(angkaBaru);
    nilai = input("Stok baru (kosong = tetap): ");
    if (!nilai.empty() && bilangan(nilai, angkaBaru) && angkaBaru <= 10000)
        barang.stok = static_cast<std::uint16_t>(angkaBaru);
    if (konfirmasi("Ubah tanggal masuk? (y/n): ")) barang.tglMasuk = inputTanggal();
    std::cout << "Data berhasil diubah.\n";
}

void hapusData(Barang data[], std::size_t& jumlah) {
    std::string kode = input("Kode barang yang akan dihapus: ");
    int indeks = cariDenganKode(data, jumlah, kode);
    if (indeks < 0) {
        std::cout << "Kode tidak ditemukan.\n";
        return;
    }
    if (konfirmasi("Yakin menghapus data? (y/n): ")) {
        hapusBarang(data, jumlah, static_cast<std::size_t>(indeks));
        std::cout << "Data berhasil dihapus.\n";
    }
}

void tampilkanStatistik(const Barang data[], std::size_t jumlah) {
    if (jumlah == 0) {
        std::cout << "Belum ada data.\n";
        return;
    }
    unsigned long long totalNilai = 0, totalHarga = 0;
    std::size_t indeksHargaMax = 0, indeksStokMin = 0;
    for (std::size_t i = 0; i < jumlah; ++i) {
        totalNilai += static_cast<unsigned long long>(data[i].harga) * data[i].stok;
        totalHarga += data[i].harga;
        if (data[i].harga > data[indeksHargaMax].harga) indeksHargaMax = i;
        if (data[i].stok < data[indeksStokMin].stok) indeksStokMin = i;
    }
    std::cout << "Jumlah jenis barang : " << jumlah << '\n'
              << "Total nilai stok   : Rp" << totalNilai << '\n'
              << "Rata-rata harga    : Rp" << totalHarga / jumlah << '\n'
              << "Harga tertinggi    : " << data[indeksHargaMax].nama
              << " (Rp" << data[indeksHargaMax].harga << ")\n"
              << "Stok terendah      : " << data[indeksStokMin].nama
              << " (" << data[indeksStokMin].stok << ")\n";
}

void tampilkanInfoMemori(const Barang data[]) {
    std::cout << "sizeof(Tanggal)       : " << sizeof(Tanggal) << " byte\n"
              << "sizeof(Barang)        : " << sizeof(Barang) << " byte\n"
              << "Kapasitas array       : " << KAPASITAS_DATA << " record\n"
              << "sizeof(array data)    : " << sizeof(Barang) * KAPASITAS_DATA << " byte\n"
              << "Memori data terpakai  : " << sizeof(Barang) * KAPASITAS_DATA
              << " byte (array tetap)\n";
    (void)data;
}

bool konfirmasi(const std::string& pertanyaan) {
    std::string jawaban = input(pertanyaan);
    return !jawaban.empty() &&
           (jawaban[0] == 'y' || jawaban[0] == 'Y');
}
