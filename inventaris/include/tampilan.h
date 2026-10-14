#ifndef TAMPILAN_H
#define TAMPILAN_H

#include "data.h"

#include <cstddef>
#include <string>

void tampilkanMenu();
void tampilkanData(const Barang data[], std::size_t jumlah);
void tambahData(Barang data[], std::size_t& jumlah);
void cariData(const Barang data[], std::size_t jumlah);
void ubahData(Barang data[], std::size_t jumlah);
void hapusData(Barang data[], std::size_t& jumlah);
void tampilkanStatistik(const Barang data[], std::size_t jumlah);
void tampilkanInfoMemori(const Barang data[]);
bool konfirmasi(const std::string& pertanyaan);

#endif
