//
// Created by pcvii on 1/18/2021.
//

#ifndef VIIIARCHIVE_JSM_SCRIPT_ENTITY_HPP
#define VIIIARCHIVE_JSM_SCRIPT_ENTITY_HPP
#include <cstdint>
#include <type_traits>
#include <utility>
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
    constexpr std::uint16_t shift = 2U;// multiply by 4 == shift by 2.
    // this is dropping the 15th bit. i guess this is okay.
    //(entryPointScript & 0x7FFF)*4; this is doing a shift and mask.
    return static_cast<std::uint16_t>(m_raw << shift) & mask;
  }
  [[nodiscard]] constexpr bool flag() const noexcept
  {
    constexpr std::uint16_t mask = 0x1U;
    constexpr std::uint16_t shift = 15U;
    return (static_cast<std::uint16_t>(m_raw >> shift) & mask) != 0;
  }

  /**
   * Get Value
   *@note required to structured binding support
   * @note can't be reference because it's a copy of 4 bits to 8 bits.
   */
  template<std::size_t I>
  requires(I < 2U) [[nodiscard]] constexpr auto get() const noexcept
  {
    if constexpr (I == 0U) {
      return position();
    } else if constexpr (I == 1U) {
      return flag();
    }
  }
};
}// namespace open_viii::field::scripts

namespace std {
/**
 * number of arguments
 * @note required to structured binding support
 */
template<>
struct tuple_size<open_viii::field::scripts::jsm_script_entity>
  : std::integral_constant<size_t, 2U>
{
};

/**
 * type of argument 0
 * @note required to structured binding support
 */
template<>
struct tuple_element<0U, open_viii::field::scripts::jsm_script_entity>
{
  using type = std::uint16_t;
};
/**
 * type of argument 1
 * @note required to structured binding support
 */
template<>
struct tuple_element<1U, open_viii::field::scripts::jsm_script_entity>
{
  using type = std::uint16_t;
};
}// namespace std
#endif// VIIIARCHIVE_JSM_SCRIPT_ENTITY_HPP
