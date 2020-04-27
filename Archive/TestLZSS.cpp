//
// Created by pcvii on 4/27/2020.
//
#include "TestLZSS.hpp"

int main()
{
  // test LZSS
  size_t i = 0;
  while (OpenVIII::Compression::LZSS::Test(i++)) {}

  //  //test lzss known issue
  //  std::vector<char> t =
  //  {0x4A,static_cast<char>(0xAA),0x25,0x59,static_cast<char>(0xC6),static_cast<char>(0x9C),0x5A,0x24,0x3D}; auto
  //  compressed = OpenVIII::Compression::LZSS::Compress(t); auto uncompressed =
  //  OpenVIII::Compression::LZSS::Decompress(compressed); if(std::equal(t.begin(),t.end(),uncompressed.begin()))
  //  {
  //    std::cout<<"Successful compress and uncompress! "<< t.size() <<" bytes\n";
  //    return 0;
  //  }
  //    std::cerr<<"Failure!\n";
  //    return 1;


  return 0;
}
