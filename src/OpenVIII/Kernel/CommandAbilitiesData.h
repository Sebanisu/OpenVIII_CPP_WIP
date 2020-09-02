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

#ifndef VIIIARCHIVE_COMMANDABILITIESDATA_H
#define VIIIARCHIVE_COMMANDABILITIESDATA_H

#include "OpenVIII/Strings/EncodedStringOffset.h"
#include "PersistentStatusesT.h"
#include "BattleOnlyStatusesT.h"
#include "AttackTypeT.h"
#include "AttackFlagsT.h"
#include "ElementT.h"
namespace open_viii::kernel {
template<LangT langVal> struct CommandAbilitiesData
{

  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Command-ability-data
   * 0x0000	2 bytes	Magic ID
   * 0x0002	2 bytes	Unknown
   * 0x0004	1 byte	Attack type
   * 0x0005	1 byte	Attack power
   * 0x0006	1 byte	Attack flags
   * 0x0007	1 byte	Hit Count
   * 0x0008	1 byte	Element
   * 0x0009	1 byte	Status attack enabler
   * 0x000A	6 bytes	Statuses
   *  public PersistentStatuses Statuses0 { get;  }
   *  public BattleOnlyStatuses Statuses1 { get;  }
   */
private:
  std::uint16_t magicID_{};
  std::uint16_t unknown_{};
  AttackTypeT attackType_{};
  std::uint8_t attackPower_{};
  AttackFlagsT attackFlags_{};
  std::uint8_t hitCount_{};
  ElementT element_{};
  std::uint8_t statusAttackEnabler_{};
  PersistentStatusesT persistentStatuses_{};
  BattleOnlyStatusesT battleOnlyStatuses_{};

public:
  [[nodiscard]] auto MagicID() const noexcept { return magicID_; }
  [[nodiscard]] auto Unknown() const noexcept { return unknown_; }
  [[nodiscard]] auto AttackType() const noexcept { return attackType_; }
  [[nodiscard]] auto AttackPower() const noexcept { return attackPower_; }
  [[nodiscard]] auto AttackFlags() const noexcept { return attackFlags_; }
  [[nodiscard]] auto HitCount() const noexcept { return hitCount_; }
  [[nodiscard]] auto Element() const noexcept { return element_; }
  [[nodiscard]] auto StatusAttackEnabler() const noexcept { return statusAttackEnabler_; }
  [[nodiscard]] auto PersistentStatuses() const noexcept { return persistentStatuses_; }
  [[nodiscard]] auto BattleOnlyStatuses() const noexcept { return battleOnlyStatuses_; }

  std::ostream &Out(std::ostream &os, [[maybe_unused]] const std::string_view &buffer) const
  {
    return os << MagicID() << ", " << Unknown() << ", " << static_cast<std::uint32_t>(AttackType()) << ", "
              << static_cast<std::uint32_t>(AttackPower()) << ", " << static_cast<std::uint32_t>(AttackFlags()) << ", "
              << static_cast<std::uint32_t>(HitCount()) << ", " << static_cast<std::uint32_t>(Element()) << ", "
              << static_cast<std::uint32_t>(StatusAttackEnabler()) << ", "
              << static_cast<std::uint32_t>(PersistentStatuses()) << ", "
              << static_cast<std::uint32_t>(BattleOnlyStatuses());
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_COMMANDABILITIESDATA_H
