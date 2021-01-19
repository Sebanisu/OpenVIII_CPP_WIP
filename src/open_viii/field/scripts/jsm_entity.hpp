//
// Created by pcvii on 1/18/2021.
//

#ifndef VIIIARCHIVE_JSM_ENTITY_HPP
#define VIIIARCHIVE_JSM_ENTITY_HPP
#include <cstdint>
namespace open_viii::field::scripts {
struct jsm_entity
{
private:
  std::uint16_t m_raw{};

public:
  constexpr jsm_entity() = default;
  [[nodiscard]] constexpr std::uint16_t raw() const noexcept
  {
    return m_raw;
  }
  [[nodiscard]] constexpr std::uint8_t script_count() const noexcept
  {
    constexpr std::uint16_t mask = 0x7FU;
    return m_raw & mask;
  }
  [[nodiscard]] constexpr std::uint16_t entry_point_entity() const noexcept
  {
    constexpr std::uint16_t mask = 0x1FF;
    constexpr std::uint16_t shift = 0x7U;
    return static_cast<std::uint16_t>(m_raw >> shift) & mask;
  }
};
}
#endif// VIIIARCHIVE_JSM_ENTITY_HPP
