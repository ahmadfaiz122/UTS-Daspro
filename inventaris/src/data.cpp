#include "data.h"

#include <algorithm> // memanggil fungsi untuk mencari, mengolah dan mengurutkan data
#include <cctype>
#include <cstring>
#include <utility>

namespace {
bool hanyaAlnum(const std::string& teks) {
    for (unsigned char karakter : teks) {
        if (!std::isalnum(karakter)) {
            return false;
        }
    }
    return true;
}
}

bool kodeValid(const std::string& kode) {
    if (kode.size() < 3 || kode.size() >= sizeof(Barang::kode) ||
        kode.rfind("BRG", 0) != 0) {
        return false;
    }
    return hanyaAlnum(kode);
}

bool tanggalValid(const Tanggal& tanggal) {
    return tanggal.hari >= 1 && tanggal.hari <= 31 &&
           tanggal.bulan >= 1 && tanggal.bulan <= 12 &&
           tanggal.tahun >= 2000 && tanggal.tahun <= 2100;
}

bool barangValid(const Barang& barang) {
    return kodeValid(barang.kode) && barang.nama[0] != '\0' &&
           barang.kategori[0] != '\0' && barang.harga <= 100000000 &&
           barang.stok <= 10000 && tanggalValid(barang.tglMasuk);
}

bool kodeSudahAda(const Barang data[], std::size_t jumlah,
                  const std::string& kode, std::size_t abaikan) {
    for (std::size_t i = 0; i < jumlah; ++i) {
        if (i != abaikan && kode == data[i].kode) {
            return true;
        }
    }
    return false;
}

int cariDenganKode(const Barang data[], std::size_t jumlah, const std::string& kode) {
    for (std::size_t i = 0; i < jumlah; ++i) {
        if (kode == data[i].kode) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

std::string keHurufKecil(const std::string& teks) {
    std::string hasil = teks;
    for (char& karakter : hasil) {
        karakter = static_cast<char>(
            std::tolower(static_cast<unsigned char>(karakter)));
    }
    return hasil;
}

bool mengandungTanpaKapital(const std::string& teks, const std::string& kunci) {
    return keHurufKecil(teks).find(keHurufKecil(kunci)) != std::string::npos;
}

void urutkanBarang(Barang data[], std::size_t jumlah, int pilihan) {
    for (std::size_t i = 0; i < jumlah; ++i) {
        std::size_t indeksMinimum = i;
        for (std::size_t j = i + 1; j < jumlah; ++j) {
            bool lebihKecil = false;
            if (pilihan == 1) {
                lebihKecil = data[j].harga < data[indeksMinimum].harga;
            } else if (pilihan == 2) {
                lebihKecil = data[j].stok < data[indeksMinimum].stok;
            } else {
                lebihKecil = keHurufKecil(data[j].nama) <
                             keHurufKecil(data[indeksMinimum].nama);
            }
            if (lebihKecil) {
                indeksMinimum = j;
            }
        }
        if (indeksMinimum != i) {
            std::swap(data[i], data[indeksMinimum]);
        }
    }
}

void hapusBarang(Barang data[], std::size_t& jumlah, std::size_t indeks) {
    if (indeks >= jumlah) {
        return;
    }
    for (std::size_t i = indeks; i + 1 < jumlah; ++i) {
        data[i] = data[i + 1];
    }
    --jumlah;
}
