//
// Created by pcvii on 8/28/2020.
//

#ifndef VIIIARCHIVE_SP1HEADER_H
#define VIIIARCHIVE_SP1HEADER_H
#include <cstdint>
#include <cstring>
#include <vector>
#include <span>
#include "sp1Offset.h"
namespace open_viii::graphics {
struct Sp1Header
{
private:
  std::uint32_t m_offset_count{};
  std::vector<Sp1Offset> m_offsets{};

public:
  Sp1Header() = default;
  explicit Sp1Header(std::span<const char> buffer)
  {
    {
      size_t sz = sizeof(std::uint32_t);
      if (sz > std::ranges::size(buffer)) {
        return;
      }
      std::memcpy(&m_offset_count, std::ranges::data(buffer), sz);
      buffer = buffer.subspan(sz);
    }
    {
      m_offsets.resize(m_offset_count);
      size_t sz = static_cast<std::size_t>(m_offset_count) * sizeof(Sp1Offset);
      if (sz > std::ranges::size(buffer)) {
        m_offset_count = {};
        m_offsets = {};
        return;
      }
      std::memcpy(std::ranges::data(m_offsets), std::ranges::data(buffer), sz);
    }
  }
  [[nodiscard]] auto size() const noexcept { return m_offset_count; }
  [[nodiscard]] const auto &offsets() const noexcept { return m_offsets; }
};

}// namespace open_viii::graphics
#endif// VIIIARCHIVE_SP1HEADER_H
