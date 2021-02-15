//
// Created by pcvii on 1/25/2021.
//
#ifndef VIIIARCHIVE_SLOT_HPP
#define VIIIARCHIVE_SLOT_HPP
#include <compare>
#include <cstdint>
namespace open_viii::kernel {
struct Slot
{
private:
  std::uint8_t m_magic_id{};
  std::uint8_t m_count{};
public:
  constexpr auto operator<=>(const Slot &right) const noexcept = default;
  [[nodiscard]] constexpr auto magic_id() const noexcept
  {
    return m_magic_id;
  }
  [[nodiscard]] constexpr auto count() const noexcept
  {
    return m_count;
  }
  friend std::ostream &operator<<(std::ostream &os, const Slot &slot)
  {
    return os << '{' << static_cast<uint16_t>(slot.magic_id()) << ", "
              << static_cast<uint16_t>(slot.count()) << '}';
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_SLOT_HPP
