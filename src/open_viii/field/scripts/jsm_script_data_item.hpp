//
// Created by pcvii on 1/19/2021.
//

#ifndef VIIIARCHIVE_JSM_SCRIPT_DATA_ITEM_HPP
#define VIIIARCHIVE_JSM_SCRIPT_DATA_ITEM_HPP
#include "opcodeT.hpp"
#include <cstdint>
#include <type_traits>
#include <utility>
namespace open_viii::field::scripts {
struct jsm_script_data_item
{
private:
  std::uint32_t m_data{};

public:
  [[nodiscard]] constexpr opcodeT opcode() const noexcept
  {
    constexpr std::uint16_t mask = 0x1FFU;// 9 bits mask
    constexpr std::uint16_t shift = 23;
    return static_cast<opcodeT>(
      static_cast<std::uint16_t>(m_data >> shift) & mask)
  }
  [[nodiscard]] constexpr std::uint32_t parameter() const noexcept
  {
    constexpr std::uint16_t mask = 0x7FFFFFU;// 23 bits mask
    return static_cast<opcodeT>(
      static_cast<std::uint16_t>(m_data >> shift) & mask)
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
      return opcode();
    } else if constexpr (I == 1U) {
      return parameter();
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
struct tuple_size<open_viii::field::scripts::jsm_script_data_item>
  : std::integral_constant<size_t, 2U>
{
};

/**
 * type of argument 0
 * @note required to structured binding support
 */
template<>
struct tuple_element<0U, open_viii::field::scripts::jsm_script_data_item>
{
  using type = open_viii::field::scripts::opcodeT;
};
/**
 * type of argument 1
 * @note required to structured binding support
 */
template<>
struct tuple_element<1U, open_viii::field::scripts::jsm_script_data_item>
{
  using type = std::uint32_t;
};
}// namespace std
#endif// VIIIARCHIVE_JSM_SCRIPT_DATA_ITEM_HPP
