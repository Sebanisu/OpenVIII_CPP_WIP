//
// Created by pcvii on 3/23/2021.
//

#ifndef VIIIARCHIVE_LZ4_LZSS_COMMON_FF8_HPP
#define VIIIARCHIVE_LZ4_LZSS_COMMON_FF8_HPP
#include "tl/input.hpp"
#include "tl/read.hpp"
#include <array>
#include <fstream>
#include <iostream>
#include <vector>
std::ofstream
  open_file(std::string_view out)
{
  std::ofstream os{};
  os.open(std::filesystem::path(out), std::ios::out | std::ios::binary);
  if (!os.is_open()) {
    std::cerr << "failed to open " << out << " for writing.";
    std::exit(1);
  }
  return os;
}
template<typename T>
void
  write_value(std::ostream &os, T i)
{
  std::array<char, sizeof(T)> tmp{};
  std::memcpy(&tmp, &i, 4);
  os.write(std::data(tmp), 4);
}
// forward declare for unique code to be in the .cpp
void compress(std::string_view in, std::string_view out);
void decompress(std::string_view in, std::string_view out);
int
  common_main(int argc, [[maybe_unused]] char *argv[])
{
  if (argc == 3) {
    compress(argv[1], argv[2]);
    return 0;
  }
  if (argc == 4 && std::string_view(argv[1]) == std::string_view("-d")) {
    decompress(argv[2], argv[3]);
    return 0;
  }
  std::cerr << "Use arguments: [-d] \"input\" \"output\"\n";
  std::cerr << "\t-d\t\t--\t required for decompress\n";
  std::cerr << "\tinput\t\t--\t input path\n";
  std::cerr << "\toutput\t\t--\t output path\n";
  return 1;
}
#endif// VIIIARCHIVE_LZ4_LZSS_COMMON_FF8_HPP
