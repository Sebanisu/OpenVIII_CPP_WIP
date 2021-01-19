//
// Created by pcvii on 1/18/2021.
//

#ifndef VIIIARCHIVE_JSM_SCRIPT_ENTITY_HPP
#define VIIIARCHIVE_JSM_SCRIPT_ENTITY_HPP
#include <cstdint>
namespace open_viii::field::scripts {
struct jsm_script_entity
{
private:
  std::uint16_t m_raw{};

public:
  constexpr jsm_script_entity() = default;
  [[nodiscard]] constexpr std::uint16_t raw() const noexcept
  {
    return m_raw;
  }
  [[nodiscard]] constexpr std::uint16_t position() const noexcept
  {
    constexpr std::uint16_t mask = 0xFFFCU;
    constexpr std::uint16_t shift = 2U; // multiply by 4 == shift by 2.
    //this is dropping the 15th bit. i guess this is okay.
    //(entryPointScript & 0x7FFF)*4; this is doing a shift and mask.
    return static_cast<std::uint16_t>(m_raw <<shift) & mask;
  }
  [[nodiscard]] constexpr bool flag () const noexcept
  {
    constexpr std::uint16_t mask = 0x1U;
    constexpr std::uint16_t shift = 15U;
    return (static_cast<std::uint16_t>(m_raw >> shift) & mask) !=0;
  }
};
}
#endif// VIIIARCHIVE_JSM_SCRIPT_ENTITY_HPP
