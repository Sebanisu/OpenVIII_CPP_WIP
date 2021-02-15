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
#include "open_viii/tools/Random.hpp"
#include <iostream>
#include <random>
#include <thread>
static void lzss_check_compress_and_decompress(const std::atomic_bool &stop)
{
  std::cout << "Testing LZSS by passing a random std::vector<char> to "
               "Compress() and Decompress()...\n";
  open_viii::tools::random_iota(
    [](const std::vector<char> &random_chars) -> bool {
      auto uncompressed = open_viii::compression::LZSS::decompress(
        open_viii::compression::LZSS::compress(random_chars));
      if (std::ranges::equal(random_chars, uncompressed)) {
        std::cout << "\rSuccessfully compressed and uncompressed data: "
                  << std::ranges::size(random_chars) << " bytes" << std::flush;
        return true;
      }
      std::cerr << "Failure!\n";
      return false;
    },
    stop);
}
static void lzss_check_decompress(const std::atomic_bool &stop)
{
  std::cout << "Testing LZSS by passing a random std::vector<char> to "
               "Decompress()...\n";
  open_viii::tools::random_iota(
    [](const std::vector<char> &random_chars) -> bool {
      auto uncompressed =
        open_viii::compression::LZSS::decompress(random_chars);
      std::cout << "\rSuccessfully uncompressed data w/o crash: "
                << std::ranges::size(random_chars) << " bytes" << std::flush;
      return true;
    },
    stop);
}
int main()
{
  std::atomic_bool stop{ false };
  {
    auto thread = std::jthread{ [&stop]() {
      lzss_check_compress_and_decompress(stop);
    } };
    std::cin.get();
    stop = true;
  }
  std::cout << '\n';
  stop = false;
  {
    auto thread = std::jthread{ [&stop]() {
      lzss_check_decompress(stop);
    } };
    std::cin.get();
    stop = true;
  }
  return 0;
}
