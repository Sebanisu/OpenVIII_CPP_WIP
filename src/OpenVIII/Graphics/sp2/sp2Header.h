//
// Created by pcvii on 8/28/2020.
//

#ifndef VIIIARCHIVE_SP2HEADER_H
#define VIIIARCHIVE_SP2HEADER_H
#include <cstdint>
#include <vector>
#include <span>
#include <ranges>
namespace open_viii::graphics {
struct Sp2Header
{
private:
  std::uint32_t m_count{};
  std::vector<std::uint32_t> m_offsets{};

public:
  Sp2Header() = default;
  explicit Sp2Header(std::span<const char> buffer)
  {
    std::memcpy(&m_count, std::ranges::data(buffer), sizeof(m_count));
    buffer = buffer.subspan(sizeof(m_count));
    m_offsets.resize(m_count);
    std::memcpy(std::ranges::data(m_offsets), std::ranges::data(buffer), sizeof(std::uint32_t) * m_count);
  }
  [[nodiscard]] const auto &offsets() const noexcept { return m_offsets; }
  [[nodiscard]] auto &size() const noexcept { return m_count; }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_SP2HEADER_H
