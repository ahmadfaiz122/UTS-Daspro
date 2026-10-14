#ifndef BERKAS_H
#define BERKAS_H

#include "data.h"

#include <string>

std::size_t bacaData(const std::string& namaBerkas, Barang data[], std::size_t kapasitas);
bool simpanData(const std::string& namaBerkas, const Barang data[], std::size_t jumlah);

#endif
