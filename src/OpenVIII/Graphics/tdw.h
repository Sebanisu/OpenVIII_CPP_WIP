//
// Created by pcvii on 8/28/2020.
//

#ifndef VIIIARCHIVE_TDW_H
#define VIIIARCHIVE_TDW_H
#include "tim.h"
#include "Bit4Values.h"
#include <cstdint>
#include <cstring>
namespace open_viii::graphics {
/**
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_TDW
 */
struct Tdw
{
private:
  static constexpr auto WIDTHS_OFFSET_VALUE = 8U;
  std::uint32_t m_widths_offset{};
  std::uint32_t m_tim_offset{};
  std::vector<Bit4Values> m_widths{};
  Tim m_tim{};
  [[nodiscard]] auto widths_size() const noexcept { return m_tim_offset - m_widths_offset; }

public:
  Tdw() = default;
  explicit Tdw([[maybe_unused]] std::span<const char> buffer)
  {
    const auto buffer_bak = buffer;
    size_t sz32 = sizeof(std::uint32_t);
    if (std::ranges::size(buffer) < sz32) {
      return;
    }
    std::memcpy(&m_widths_offset, std::ranges::data(buffer), sz32);
    buffer = buffer.subspan(sz32);
    if (WIDTHS_OFFSET_VALUE != m_widths_offset) {
      m_widths_offset = 0U;
      return;
    }
    if (std::ranges::size(buffer) < sz32) {
      return;
    }
    std::memcpy(&m_tim_offset, std::ranges::data(buffer), sz32);
    buffer = buffer.subspan(sz32);

    if (std::ranges::size(buffer) < widths_size()) {
      return;
    }
    m_widths.resize(widths_size());
    std::memcpy(std::ranges::data(m_widths), std::ranges::data(buffer_bak.subspan(m_widths_offset)), widths_size());
    m_tim = Tim{ buffer_bak.subspan(m_tim_offset) };
  }
  [[nodiscard]] auto operator[](size_t i) const
  {
    bool first = (i % 2U == 0U);
    i /= 2U;
    if (first) {
      return static_cast<std::uint8_t>(m_widths.at(i).first);
    }
    return static_cast<std::uint8_t>(m_widths.at(i).second);
  }
  [[nodiscard]] auto size() const { return std::ranges::size(m_widths) * 2U; }
  [[nodiscard]] const Tim &tim() const { return m_tim; }
  friend std::ostream &operator<<(std::ostream &os, const Tdw &t)
  {
    os << t.size() << " char widths: ";
    for (const Bit4Values &w : t.m_widths) {
      os << static_cast<std::uint32_t>(w.first) << ", " << static_cast<std::uint32_t>(w.second) << ", ";
    }
    return os << '\n' << t.m_tim;
  }
  void save(const std::string_view &path) { m_tim.save(path); }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_TDW_H
