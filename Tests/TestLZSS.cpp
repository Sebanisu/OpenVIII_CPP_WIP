//
// Created by pcvii on 4/27/2020.
//
#include "TestLZSS.h"
#include <random>

int main()
{
  // test LZSS
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<unsigned char> dis(0U);
  const auto Test = [&dis,&gen](const size_t &size)
  {
    if (size <= 0) { return true; }
    std::vector<char> vecOfRandomNums = std::vector<char>(static_cast<unsigned int>(size));
    if (vecOfRandomNums.empty()) { return true; }
    std::generate(vecOfRandomNums.begin(), vecOfRandomNums.end(), [&dis,&gen]() {
           return static_cast<char>(dis(gen));
    });
    auto compressed = OpenVIII::Compression::LZSS::Compress(vecOfRandomNums);
    auto uncompressed = OpenVIII::Compression::LZSS::Decompress(compressed);
    if (std::equal(vecOfRandomNums.begin(), vecOfRandomNums.end(), uncompressed.begin())) {
      std::cout << "Successful compress and uncompress! " << size << " bytes\n";
      return true;
    }
    std::cerr << "Failure!\n";
    return false;
  };

  size_t i = 0;
  while (Test(i++)) {}



  return 0;
}
