//
// Created by pcvii on 4/27/2020.
//

#ifndef VIIICPPTEST_L4Z_HPP
#define VIIICPPTEST_L4Z_HPP
namespace OpenVIII::Compression {
struct L4Z
{
  [[nodiscard]] [[maybe_unused]] static auto Decompress(const std::vector<char> &data, const size_t &size = 0);
};
}// namespace OpenVIII::Compression
#endif// VIIICPPTEST_L4Z_HPP
