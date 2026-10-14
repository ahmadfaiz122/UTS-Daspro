#include "berkas.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

namespace {
bool angka(const std::string& teks, unsigned long long& hasil) {
    if (teks.empty()) {
        return false;
    }
    std::stringstream stream(teks);
    char sisa = '\0';
    if (!(stream >> hasil) || (stream >> sisa)) {
        return false;
    }
    return true;
}

bool parseBaris(const std::string& baris, Barang& barang) {
    std::stringstream stream(baris);
    std::string kode, nama, kategori, harga, stok, hari, bulan, tahun;
    if (!std::getline(stream, kode, '|') || !std::getline(stream, nama, '|') ||
        !std::getline(stream, kategori, '|') || !std::getline(stream, harga, '|') ||
        !std::getline(stream, stok, '|') || !std::getline(stream, hari, '|') ||
        !std::getline(stream, bulan, '|') || !std::getline(stream, tahun, '|')) {
        return false;
    }
    std::string sisa;
    if (std::getline(stream, sisa)) {
        return false;
    }

    unsigned long long nilaiHarga, nilaiStok, nilaiHari, nilaiBulan, nilaiTahun;
    if (!angka(harga, nilaiHarga) || !angka(stok, nilaiStok) ||
        !angka(hari, nilaiHari) || !angka(bulan, nilaiBulan) ||
        !angka(tahun, nilaiTahun) || nilaiHarga > 100000000 ||
        nilaiStok > 10000 || nilaiHari > 255 || nilaiBulan > 255 ||
        nilaiTahun > 65535) {
        return false;
    }
    if (!kodeValid(kode) || nama.empty() || kategori.empty()) {
        return false;
    }
    Barang hasil{};
    hasil.harga = static_cast<std::uint32_t>(nilaiHarga);
    hasil.stok = static_cast<std::uint16_t>(nilaiStok);
    hasil.tglMasuk = {static_cast<std::uint8_t>(nilaiHari),
                      static_cast<std::uint8_t>(nilaiBulan),
                      static_cast<std::uint16_t>(nilaiTahun)};
    if (kode.size() >= sizeof(hasil.kode) || nama.size() >= sizeof(hasil.nama) ||
        kategori.size() >= sizeof(hasil.kategori)) {
        return false;
    }
    std::copy(kode.begin(), kode.end(), hasil.kode);
    std::copy(nama.begin(), nama.end(), hasil.nama);
    std::copy(kategori.begin(), kategori.end(), hasil.kategori);
    hasil.kode[kode.size()] = '\0';
    hasil.nama[nama.size()] = '\0';
    hasil.kategori[kategori.size()] = '\0';
    if (!barangValid(hasil)) {
        return false;
    }
    barang = hasil;
    return true;
}
}

std::size_t bacaData(const std::string& namaBerkas, Barang data[], std::size_t kapasitas) {
    std::ifstream berkas(namaBerkas);
    if (!berkas) {
        std::cerr << "Peringatan: berkas data tidak dapat dibuka: " << namaBerkas << '\n';
        return 0;
    }
    std::size_t jumlah = 0;
    std::string baris;
    std::size_t nomorBaris = 0;
    while (std::getline(berkas, baris)) {
        ++nomorBaris;
        if (baris.empty()) {
            continue;
        }
        Barang barang{};
        if (!parseBaris(baris, barang) ||
            kodeSudahAda(data, jumlah, barang.kode)) {
            std::cerr << "Peringatan: baris " << nomorBaris
                      << " diabaikan karena tidak valid atau duplikat.\n";
            continue;
        }
        if (jumlah == kapasitas) {
            std::cerr << "Peringatan: kapasitas data penuh; baris berikutnya diabaikan.\n";
            break;
        }
        data[jumlah++] = barang;
    }
    return jumlah;
}

bool simpanData(const std::string& namaBerkas, const Barang data[], std::size_t jumlah) {
    std::ofstream berkas(namaBerkas);
    if (!berkas) {
        std::cerr << "Gagal menyimpan data ke " << namaBerkas << ".\n";
        return false;
    }
    for (std::size_t i = 0; i < jumlah; ++i) {
        berkas << data[i].kode << '|' << data[i].nama << '|' << data[i].kategori
               << '|' << data[i].harga << '|' << data[i].stok << '|'
               << static_cast<unsigned>(data[i].tglMasuk.hari) << '|'
               << static_cast<unsigned>(data[i].tglMasuk.bulan) << '|'
               << data[i].tglMasuk.tahun << '\n';
    }
    return true;
}
