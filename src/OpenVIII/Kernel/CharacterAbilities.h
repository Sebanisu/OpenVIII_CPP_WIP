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

#ifndef VIIIARCHIVE_CHARACTERABILITIES_H
#define VIIIARCHIVE_CHARACTERABILITIES_H

#include "OpenVIII/Strings/EncodedStringOffset.h"
#include "CharacterAbilityFlagsT.h"
#include <cstring>
namespace open_viii::kernel {
template<LangT langVal> struct CharacterAbilities
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Character-abilities
   * 0x0000	2 bytes	Offset to ability name
   * 0x0002	2 bytes	Offset to ability description
   * 0x0004	1 byte	AP Required to learn ability
   * 0x0005	3 byte	Flags
   */
private:
  EncodedStringOffset m_name_offset{};
  EncodedStringOffset m_description_offset{};
  std::uint8_t m_ap_required{};
  // uint32_t characterAbilityFlags_ : 3;// cpp20 allows default member initializers for bitfields add {} in cpp20.
  std::array<std::uint8_t, 3> m_character_ability_flags{};

public:
  [[maybe_unused]] [[nodiscard]] auto &name_offset() const noexcept { return m_name_offset; }
  [[maybe_unused]] [[nodiscard]] auto &description_offset() const noexcept { return m_description_offset; }

  [[maybe_unused]] [[nodiscard]] auto ap_required() const noexcept { return m_ap_required; }
  [[nodiscard]] auto character_ability_flags() const
  {
    // I think this is okay.
    // The size of the enum is 4 bytes but the field is 3 bytes.
    // return static_cast<CharacterAbilityFlagsT>(characterAbilityFlags_);
    CharacterAbilityFlagsT out{};
    std::memcpy(&out, m_character_ability_flags.data(), m_character_ability_flags.size());
    // out = static_cast<CharacterAbilityFlagsT>(static_cast<uint32_t>(out) << 1U);
    return out;
  }
  std::ostream &out(std::ostream &os, const std::string_view &buffer) const
  {
    auto name = m_name_offset.decoded_string<langVal>(buffer);
    auto description = m_description_offset.decoded_string<langVal>(buffer);
    if (!std::empty(name)) {
      os << Tools::u8_to_sv(name);
    }
    if (!std::empty(description)) {
      os << ", " << Tools::u8_to_sv(description);
    }

    auto test = character_ability_flags();
    return os << ", " << static_cast<std::uint32_t>(m_ap_required) << ", " << static_cast<uint32_t>(test);
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_CHARACTERABILITIES_H
