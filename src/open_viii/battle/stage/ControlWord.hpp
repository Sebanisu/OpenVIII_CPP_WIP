//
// Created by pcvii on 12/16/2020.
//
#ifndef VIIIARCHIVE_CONTROLWORD_HPP
#define VIIIARCHIVE_CONTROLWORD_HPP
#include "FovTypeT.hpp"
#include "LayoutTypeT.hpp"
#include "RollTypeT.hpp"
#include <cstdint>
namespace open_viii::battle::stage {
struct ControlWord
{
private:
  std::uint16_t                  m_word{};
  constexpr static std::uint16_t END_VAL = 0xFFFF;

public:
  constexpr ControlWord() = default;
  constexpr explicit ControlWord(uint16_t word) : m_word(word) {}
  [[nodiscard]] constexpr bool end() const noexcept
  {
    return m_word == END_VAL;
  }
  [[nodiscard]] constexpr auto fov() const noexcept
  {
    constexpr std::uint16_t shift = 0x6U;
    constexpr std::uint8_t  mask  = 0x3U;
    return static_cast<FovTypeT>(static_cast<std::uint16_t>(m_word >> shift)
                                 & mask);
  }
  [[nodiscard]] constexpr auto roll() const noexcept
  {
    constexpr std::uint16_t shift = 0x8U;
    constexpr std::uint8_t  mask  = 0x3U;
    return static_cast<RollTypeT>(static_cast<std::uint16_t>(m_word >> shift)
                                  & mask);
  }
  [[nodiscard]] constexpr auto layout() const noexcept
  {
    constexpr std::uint8_t mask = 0x1U;
    return static_cast<LayoutTypeT>(m_word & mask);
  }
};
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_CONTROLWORD_HPP
