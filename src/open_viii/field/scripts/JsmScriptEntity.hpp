//
// Created by pcvii on 1/18/2021.
//
#ifndef VIIIARCHIVE_JSMSCRIPTENTITY_HPP
#define VIIIARCHIVE_JSMSCRIPTENTITY_HPP
namespace open_viii::field::scripts {
struct JsmScriptEntity
{
private:
  std::uint16_t m_raw{};

public:
  constexpr JsmScriptEntity() = default;
  [[nodiscard]] constexpr std::uint16_t
    raw() const noexcept
  {
    return m_raw;
  }
  [[nodiscard]] constexpr std::uint16_t
    position() const noexcept
  {
    /**
     * they use 0x7FFF but once you shift over 2 or multiply by 4 the left 2
     * bits fall off. my mask is applied after shifting looks like
     * 0b1111'1111'1111'1100
     * @see
     * https://github.com/myst6re/deling/blob/develop/src/files/JsmFile.cpp#L154
     */
    constexpr std::uint16_t mask  = 0xFFFCU;
    /**
     * multiply by 4 == left shift by 2.
     * @see
     * https://github.com/myst6re/deling/blob/develop/src/files/JsmFile.cpp#L155
     */
    constexpr std::uint16_t shift = 2U;// m
    // this is dropping the 15th bit. i guess this is okay.
    //(entryPointScript & 0x7FFF)*4; this is doing a shift and mask.
    return static_cast<std::uint16_t>(m_raw << shift) & mask;
  }
  [[nodiscard]] constexpr bool
    flag() const noexcept
  {
    constexpr std::uint16_t mask  = 0x1U;
    constexpr std::uint16_t shift = 15U;
    return (static_cast<std::uint16_t>(m_raw >> shift) & mask) != 0;
  }
  /**
   * This bit is lost in the transfer. Unsure if it's just unimportant.
   * @return
   */
  [[nodiscard]] constexpr bool
    unk() const noexcept
  {
    constexpr std::uint16_t mask  = 0x1U;
    constexpr std::uint16_t shift = 14U;
    return (static_cast<std::uint16_t>(m_raw >> shift) & mask) != 0;
  }
  /**
   * Get Value
   *@note required to structured binding support

   */
  template<std::size_t I>
  requires(I < 2U) [[nodiscard]] constexpr auto get() const noexcept
  {
    if constexpr (I == 0U) {
      return position();
    }
    else if constexpr (I == 1U) {
      return flag();
    }
  }
};
}// namespace open_viii::field::scripts

/**
 * number of arguments
 * @note required to structured binding support
 */
template<>
struct [[maybe_unused]] std::tuple_size<
  open_viii::field::scripts::JsmScriptEntity>
  : std::integral_constant<size_t, 2U>
{
};
/**
 * type of argument 0
 * @note required to structured binding support
 */
template<>
struct [[maybe_unused]] std::
  tuple_element<0U, open_viii::field::scripts::JsmScriptEntity>
{
  using type = std::uint16_t;
};
/**
 * type of argument 1
 * @note required to structured binding support
 */
template<>
struct [[maybe_unused]] std::
  tuple_element<1U, open_viii::field::scripts::JsmScriptEntity>
{
  using type = std::uint16_t;
};

#endif// VIIIARCHIVE_JSMSCRIPTENTITY_HPP
