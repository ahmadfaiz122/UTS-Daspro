#include "berkas.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

namespace {
bool angka(const string& teks, unsigned long long& hasil) {
    if (teks.empty()) {
        return false;
    }
    stringstream stream(teks);
    char sisa = '\0';
    if (!(stream >> hasil) || (stream >> sisa)) {
        return false;
    }
    return true;
}

bool parseBaris(const string& baris, Barang& barang) {
    stringstream stream(baris);
    string kode, nama, kategori, harga, stok, hari, bulan, tahun;
    if (!getline(stream, kode, '|') || !getline(stream, nama, '|') ||
        !getline(stream, kategori, '|') || !getline(stream, harga, '|') ||
        !getline(stream, stok, '|') || !getline(stream, hari, '|') ||
        !getline(stream, bulan, '|') || !getline(stream, tahun, '|')) {
        return false;
    }
    string sisa;
    if (getline(stream, sisa)) {
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
    hasil.harga = static_cast<uint32_t>(nilaiHarga);
    hasil.stok = static_cast<uint16_t>(nilaiStok);
    hasil.tglMasuk = {static_cast<uint8_t>(nilaiHari),
                      static_cast<uint8_t>(nilaiBulan),
                      static_cast<uint16_t>(nilaiTahun)};
    if (kode.size() >= sizeof(hasil.kode) || nama.size() >= sizeof(hasil.nama) ||
        kategori.size() >= sizeof(hasil.kategori)) {
        return false;
    }
    copy(kode.begin(), kode.end(), hasil.kode);
    copy(nama.begin(), nama.end(), hasil.nama);
    copy(kategori.begin(), kategori.end(), hasil.kategori);
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

size_t bacaData(const string& namaBerkas, Barang data[], size_t kapasitas) {
    ifstream berkas(namaBerkas);
    if (!berkas) {
        cerr << "Peringatan: berkas data tidak dapat dibuka: " << namaBerkas << '\n';
        return 0;
    }
    size_t jumlah = 0;
    string baris;
    size_t nomorBaris = 0;
    while (getline(berkas, baris)) {
        ++nomorBaris;
        if (baris.empty()) {
            continue;
        }
        Barang barang{};
        if (!parseBaris(baris, barang) ||
            kodeSudahAda(data, jumlah, barang.kode)) {
            cerr << "Peringatan: baris " << nomorBaris
                      << " diabaikan karena tidak valid atau duplikat.\n";
            continue;
        }
        if (jumlah == kapasitas) {
            cerr << "Peringatan: kapasitas data penuh; baris berikutnya diabaikan.\n";
            break;
        }
        data[jumlah++] = barang;
    }
    return jumlah;
}

bool simpanData(const string& namaBerkas, const Barang data[], size_t jumlah) {
    ofstream berkas(namaBerkas);
    if (!berkas) {
        cerr << "Gagal menyimpan data ke " << namaBerkas << ".\n";
        return false;
    }
    for (size_t i = 0; i < jumlah; ++i) {
        berkas << data[i].kode << '|' << data[i].nama << '|' << data[i].kategori
               << '|' << data[i].harga << '|' << data[i].stok << '|'
               << static_cast<unsigned>(data[i].tglMasuk.hari) << '|'
               << static_cast<unsigned>(data[i].tglMasuk.bulan) << '|'
               << data[i].tglMasuk.tahun << '\n';
    }
    return true;
}
