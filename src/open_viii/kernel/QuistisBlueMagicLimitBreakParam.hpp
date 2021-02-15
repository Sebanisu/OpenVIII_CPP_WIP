//
// Created by pcvii on 1/25/2021.
//
#ifndef VIIIARCHIVE_QUISTISBLUEMAGICLIMITBREAKPARAM_HPP
#define VIIIARCHIVE_QUISTISBLUEMAGICLIMITBREAKPARAM_HPP
#include "BattleOnlyStatusesT.hpp"
#include "CrisisLevelT.hpp"
#include "PersistentStatusesT.hpp"
#include <array>
#include <compare>
#include <iostream>
namespace open_viii::kernel {
/**
 * Offset	Length	Description
 * 0x0000	4 bytes	Status 1
 * 0x0004	2 bytes	Status 0
 * 0x0006	1 bytes	Attack Power
 * 0x0007	1 byte	Attack Param
 * @see
 * https://github.com/DarkShinryu/doomtrain/wiki/Quistis-limit-break-parameters
 */
struct QuistisBlueMagicLimitBreakParam
{
private:
  BattleOnlyStatusesT m_battle_only_statuses{};
  PersistentStatusesT m_persistent_statuses{};
  std::uint8_t        m_attack_power{};
  std::uint8_t        m_attack_param{};
public:
  constexpr auto operator<=>(
    const QuistisBlueMagicLimitBreakParam &right) const noexcept = default;
  [[nodiscard]] constexpr auto battle_only_statuses() const noexcept
  {
    return m_battle_only_statuses;
  }
  [[nodiscard]] constexpr auto persistent_statuses() const noexcept
  {
    return m_persistent_statuses;
  }
  [[nodiscard]] constexpr auto attack_power() const noexcept
  {
    return m_attack_power;
  }
  [[nodiscard]] constexpr auto attack_param() const noexcept
  {
    return m_attack_param;
  }
  std::ostream &out(std::ostream &                                os,
                    [[maybe_unused]] const std::span<const char> &buffer) const
  {
    return os << '{' << static_cast<std::uint32_t>(battle_only_statuses())
              << ", " << static_cast<std::uint32_t>(persistent_statuses())
              << ", " << static_cast<std::uint32_t>(attack_power()) << ", "
              << static_cast<std::uint32_t>(attack_param()) << '}';
  }
  friend std::ostream &operator<<(std::ostream &                         os,
                                  const QuistisBlueMagicLimitBreakParam &input)
  {
    return input.out(os, "");
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_QUISTISBLUEMAGICLIMITBREAKPARAM_HPP
