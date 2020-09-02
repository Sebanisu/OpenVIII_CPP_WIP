//
// Created by pcvii on 8/28/2020.
//

#ifndef VIIIARCHIVE_SP1OFFSET_H
#define VIIIARCHIVE_SP1OFFSET_H
#include <cstdint>
namespace open_viii::graphics {
struct Sp1Offset
{
private:
  std::uint16_t m_offset{};
  std::uint16_t m_count{};

public:
  Sp1Offset() = default;
  [[nodiscard]] auto offset() const noexcept { return m_offset; }
  [[nodiscard]] auto count() const noexcept { return m_count; }
};

}// namespace open_viii::graphics
#endif// VIIIARCHIVE_SP1OFFSET_H
