#ifndef DATA_H
#define DATA_H

#include <cstddef>
#include <cstdint>
#include <string>

constexpr std::size_t KAPASITAS_DATA = 100;

struct Tanggal {
    std::uint8_t hari;
    std::uint8_t bulan;
    std::uint16_t tahun;
};

struct Barang {
    std::uint32_t harga;
    Tanggal tglMasuk;
    std::uint16_t stok;
    char kode[8];
    char nama[30];
    char kategori[15];
};

bool kodeValid(const std::string& kode);
bool tanggalValid(const Tanggal& tanggal);
bool barangValid(const Barang& barang);
bool kodeSudahAda(const Barang data[], std::size_t jumlah,
                  const std::string& kode, std::size_t abaikan = KAPASITAS_DATA);
int cariDenganKode(const Barang data[], std::size_t jumlah, const std::string& kode);
void urutkanBarang(Barang data[], std::size_t jumlah, int pilihan);
void hapusBarang(Barang data[], std::size_t& jumlah, std::size_t indeks);
std::string keHurufKecil(const std::string& teks);
bool mengandungTanpaKapital(const std::string& teks, const std::string& kunci);

#endif
