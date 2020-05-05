//
// Created by pcvii on 4/27/2020.
//

#ifndef VIIICPPTEST_L4Z_H
#define VIIICPPTEST_L4Z_H
namespace OpenVIII::Compression {
struct L4Z
{
  [[nodiscard]] [[maybe_unused]] static std::vector<char> Decompress(const char * data,const size_t & dataSize, const size_t &size = 0);
};
}// namespace OpenVIII::Compression
#endif// VIIICPPTEST_L4Z_H
