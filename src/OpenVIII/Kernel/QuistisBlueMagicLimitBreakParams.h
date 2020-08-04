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
namespace OpenVIII::Kernel { struct QuistisBlueMagicLimitBreakParam
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
  BattleOnlyStatusesT BattleOnlyStatuses_{};
  PersistentStatusesT PersistentStatuses_{};
  std::uint8_t AttackPower_{};
  std::uint8_t AttackParam_{};

public:
  [[nodiscard]] auto BattleOnlyStatuses() const noexcept { return BattleOnlyStatuses_; }
  [[nodiscard]] auto PersistentStatuses() const noexcept { return PersistentStatuses_; }
  [[nodiscard]] auto AttackPower() const noexcept { return AttackPower_; }
  [[nodiscard]] auto AttackParam() const noexcept { return AttackParam_; }
  std::ostream &Out(std::ostream &os, [[maybe_unused]] const std::string_view &buffer) const
  {
    return os << '{' << static_cast<std::uint32_t>(BattleOnlyStatuses()) << ", "
              << static_cast<std::uint32_t>(PersistentStatuses()) << ", " << static_cast<std::uint32_t>(AttackPower())
              << ", " << static_cast<std::uint32_t>(AttackParam()) << '}';
  }
  friend std::ostream &operator<<(std::ostream &os, const QuistisBlueMagicLimitBreakParam &input)
  {
    return input.Out(os, "");
  }
};
struct QuistisBlueMagicLimitBreakParams
{
private:
  CrisisLevelT<QuistisBlueMagicLimitBreakParam> blueMagicData_{};

public:
  [[nodiscard]] const auto *operator->() const noexcept { return &blueMagicData_; }
  std::ostream &Out(std::ostream &os, [[maybe_unused]] const std::string_view &buffer) const
  {
    return os << blueMagicData_;
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_QUISTISBLUEMAGICLIMITBREAKPARAMS_H
