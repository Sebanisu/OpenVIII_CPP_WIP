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

#ifndef VIIIARCHIVE_DEVOUR_H
#define VIIIARCHIVE_DEVOUR_H
#include "OpenVIII/Strings/EncodedStringOffset.h"
#include "BattleOnlyStatusesT.h"
#include "PersistentStatusesT.h"
namespace open_viii::Kernel {
enum class PercentQuantityT : std::uint8_t {
  // 0% = 0x00
  None [[maybe_unused]] = 0x00,

  // 6.25% = 0x01,
  Sixteenth = 0x01,

  // 12.50% = 0x02
  Eighth = 0x02,

  // 25% = 0x04
  Quarter = 0x04,

  // 50% = 0x08
  Half = 0x08,

  // 100% = 0x10
  Full = 0x10,
};
enum class DevourStatFlagT : std::uint8_t {
  None = 0x00,
  STR = 0x01,
  VIT = 0x02,
  MAG = 0x04,
  SPR = 0x08,
  SPD = 0x10,
};
template<LangT langVal> struct Devour
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Devour
   * Offset	Length	Description
   * 0x0000	2 bytes	Offset to devour description
   * 0x0002	1 byte	Damage or heal HP and Status
   *   0x1E - Cure
   *   0x1F - Damage
   * 0x0003	1 byte	HP Heal/DMG Quantity Flag
   *   0x00 - 0%
   *   0x01 - 6.25%
   *   0x02 - 12.50%
   *   0x04 - 25%
   *   0x08 - 50%
   *   0x10 - 100%
   * 0x0004	4 bytes	status_1; //statuses 8-39
   * 0x0008	2 bytes	status_0; //statuses 0-7
   * 0x000A	1 byte	Raised Stat Flag
   *   0x00 - None
   *   0x01 - STR
   *   0x02 - VIT
   *   0x04 - MAG
   *   0x08 - SPR
   *   0x10 - SPD
   * 0x000B	1 byte	Raised Stat HP Quantity
   * */
private:
  EncodedStringOffset descriptionOffset_{};
  std::uint8_t damageOrHeal_{};// HP and Status; If last on right bit is set heal, and if not dmg. Rest looks like
                               // 0b‭00011110‬.
  PercentQuantityT percentQuantity_{};
  BattleOnlyStatusesT battleOnlyStatuses_{};// statuses 8-39
  PersistentStatusesT persistentStatuses_{};// statuses 0-7
  DevourStatFlagT devourStatFlag_{};
  std::uint8_t raisedStatHPQuantity_{};

public:
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  [[nodiscard]] auto DamageOrHeal() const noexcept
  {
    return (damageOrHeal_ & 0x1U) == 0;
  }// HP and Status //false is damage, true is heal.
  [[nodiscard]] auto PercentQuantity() const noexcept// clang tidy thinks this can be made static...
  {
    const auto FlagSet = [this](const PercentQuantityT &flag) {
      return (static_cast<uint8_t>(percentQuantity_) & static_cast<uint8_t>(flag)) != 0;
    };
    float out{};
    if (FlagSet(PercentQuantityT::Full)) {
      static constexpr auto full = 1.0F;
      out += full;
    }
    if (FlagSet(PercentQuantityT::Half)) {
      static constexpr auto half = 1.0F / 2.0F;
      out += half;
    }
    if (FlagSet(PercentQuantityT::Quarter)) {
      static constexpr auto quarter = 1.0F / 4.0F;
      out += quarter;
    }
    if (FlagSet(PercentQuantityT::Eighth)) {
      static constexpr auto eighth = 1.0F / 8.0F;
      out += eighth;
    }
    if (FlagSet(PercentQuantityT::Sixteenth)) {
      static constexpr auto sixteenth = 1.0F / 16.0F;
      out += sixteenth;
    }
    return out;
  }
  [[nodiscard]] auto BattleOnlyStatuses() const noexcept { return battleOnlyStatuses_; }// statuses 8-39
  [[nodiscard]] auto PersistentStatuses() const noexcept { return persistentStatuses_; }// statuses 0-7
  [[nodiscard]] auto DevourStatFlag() const noexcept { return devourStatFlag_; }
  [[nodiscard]] auto RaisedStatHPQuantity() const noexcept { return raisedStatHPQuantity_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer) const
  {
    auto description = descriptionOffset_.decoded_string<langVal>(buffer);
    if (!std::empty(description)) {
      os << Tools::u8tosv(description);
    }
    return os << ", " << static_cast<std::uint32_t>(DamageOrHeal()) << ", " << PercentQuantity() << ", "
              << static_cast<std::uint32_t>(BattleOnlyStatuses()) << ", "
              << static_cast<std::uint32_t>(PersistentStatuses()) << ", "
              << static_cast<std::uint32_t>(DevourStatFlag()) << ", "
              << static_cast<std::uint32_t>(RaisedStatHPQuantity());
  }
};
}// namespace open_viii::Kernel
#endif// VIIIARCHIVE_DEVOUR_H
