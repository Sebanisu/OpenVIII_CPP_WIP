// This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#include "TestLZSS.hpp"
#include "open_viii/compression/LZSS.hpp"
#include <iostream>
#include <random>
#include <thread>
static void fuzz_loop_check_compress_and_decompress(const std::atomic_bool &stop)
{

  // test LZSS
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<unsigned char> dis(0U);

  size_t i{ 0 };
  std::cout << "Testing LZSS by passing a random std::vector<char> to Compress() and Decompress()...\n";
  static const auto run_once = [&dis, &gen](const size_t &size) {
    if (size <= 0) {
      return true;
    }
    std::vector<char> random_chars = std::vector<char>(size);
    if (random_chars.empty()) {
      return true;
    }
    std::generate(random_chars.begin(), random_chars.end(), [&dis, &gen]() {
      return static_cast<char>(dis(gen));
    });
    // instead of storing compressed variable we just pass it directly into Decompress();
    auto uncompressed = open_viii::compression::LZSS::decompress(open_viii::compression::LZSS::compress(random_chars));
    if (std::equal(random_chars.begin(), random_chars.end(), uncompressed.begin())) {
      std::cout << "\rSuccessfully compressed and uncompressed data: " << size << " bytes" << std::flush;
      return true;
    }
    std::cerr << "Failure!\n";
    return false;
  };
  while (!stop && run_once(i++)) {}// inplace lambda
}

static void fuzz_loop_decompress_lzss(const std::atomic_bool &stop)
{

  // test LZSS
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<unsigned char> dis(0U);

  size_t i{ 0 };
  std::cout << "Testing LZSS by passing a random std::vector<char> to Decompress()...\n";
  static const auto run_once = [&dis, &gen](const size_t &size) {
    if (size <= 0) {
      return true;
    }
    std::vector<char> random_chars = std::vector<char>(size);
    if (random_chars.empty()) {
      return true;
    }
    std::generate(random_chars.begin(), random_chars.end(), [&dis, &gen]() {
      return static_cast<char>(dis(gen));
    });
    // decompress random input see if a error happens.
    auto uncompressed = open_viii::compression::LZSS::decompress(random_chars);
    std::cout << "\rSuccessfully uncompressed data w/o crash: " << size << " bytes" << std::flush;
    return true;
  };
  while (!stop && run_once(i++)) {}// inplace lambda
}

//static void fuzz_loop_decompress_lz4(const std::atomic_bool &stop)
//{
//
//  // test LZSS
//  std::random_device rd;
//  std::mt19937 gen(rd());
//  std::uniform_int_distribution<unsigned char> dis(0U);
//
//  size_t i{ 0 };
//  std::cout << "Testing LZ4 by passing a random std::vector<char> to Decompress()...\n";
//  static const auto run_once = [&dis, &gen](const size_t &size) {
//    if (size <= 0) {
//      return true;
//    }
//    std::vector<char> random_chars = std::vector<char>(size);
//    if (random_chars.empty()) {
//      return true;
//    }
//    std::generate(random_chars.begin(), random_chars.end(), [&dis, &gen]() {
//      return static_cast<char>(dis(gen));
//    });
//    // decompress random input see if a error happens. Unsure if this is doing anything because LZ4 expects things.
//    auto uncompressed = open_viii::compression::l4z::decompress(random_chars, size);// LZ4 expects to know the size.
//    std::cout << "\rSuccessfully uncompressed data w/o crash: " << size << " bytes" << std::flush;
//    return true;
//  };
//  while (!stop && run_once(i++)) {}// inplace lambda
//}
int main()
{
  std::atomic_bool stop{ false };
  {
    auto thread = std::jthread{ [&stop]() {
      fuzz_loop_check_compress_and_decompress(stop);
    } };
    std::cin.get();
    stop = true;
  }
  std::cout << '\n';
  stop = false;
  {
    auto thread = std::jthread{ [&stop]() {
      fuzz_loop_decompress_lzss(stop);
    } };
    std::cin.get();
    stop = true;
  }

//  std::cout << '\n';
//  stop = false;
//  {
//    auto thread = std::jthread{ [&stop]() {
//      fuzz_loop_decompress_lz4(stop);
//    } };
//    std::cin.get();
//    stop = true;
//  }

  return 0;
}
