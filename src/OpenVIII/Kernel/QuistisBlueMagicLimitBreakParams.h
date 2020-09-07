// This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#ifndef VIIIARCHIVE_QUISTISBLUEMAGICLIMITBREAKPARAMS_H
#define VIIIARCHIVE_QUISTISBLUEMAGICLIMITBREAKPARAMS_H
#include "CrisisLevelT.h"
#include "BattleOnlyStatusesT.h"
#include "PersistentStatusesT.h"
#include <array>
#include <iostream>
namespace open_viii::kernel {
struct QuistisBlueMagicLimitBreakParam
{

  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Quistis-limit-break-parameters
   * Offset	Length	Description
   * 0x0000	4 bytes	Status 1
   * 0x0004	2 bytes	Status 0
   * 0x0006	1 bytes	Attack Power
   * 0x0007	1 byte	Attack Param
   */
private:
  BattleOnlyStatusesT m_battle_only_statuses{};
  PersistentStatusesT m_persistent_statuses{};
  std::uint8_t m_attack_power{};
  std::uint8_t m_attack_param{};

public:
  [[nodiscard]] auto battle_only_statuses() const noexcept { return m_battle_only_statuses; }
  [[nodiscard]] auto persistent_statuses() const noexcept { return m_persistent_statuses; }
  [[nodiscard]] auto attack_power() const noexcept { return m_attack_power; }
  [[nodiscard]] auto attack_param() const noexcept { return m_attack_param; }
  std::ostream &out(std::ostream &os, [[maybe_unused]] const std::span<const char> &buffer) const
  {
    return os << '{' << static_cast<std::uint32_t>(battle_only_statuses()) << ", "
              << static_cast<std::uint32_t>(persistent_statuses()) << ", " << static_cast<std::uint32_t>(attack_power())
              << ", " << static_cast<std::uint32_t>(attack_param()) << '}';
  }
  friend std::ostream &operator<<(std::ostream &os, const QuistisBlueMagicLimitBreakParam &input)
  {
    return input.out(os, "");
  }
};
struct QuistisBlueMagicLimitBreakParams
{
private:
  CrisisLevelT<QuistisBlueMagicLimitBreakParam> m_blue_magic_data{};

public:
  [[nodiscard]] const auto *operator->() const noexcept { return &m_blue_magic_data; }
  std::ostream &out(std::ostream &os, [[maybe_unused]] const std::span<const char> &buffer) const
  {
    return os << m_blue_magic_data;
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_QUISTISBLUEMAGICLIMITBREAKPARAMS_H
