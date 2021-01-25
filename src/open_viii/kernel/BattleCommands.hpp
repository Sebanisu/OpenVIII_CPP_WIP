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

#ifndef VIIIARCHIVE_BATTLECOMMANDS_HPP
#define VIIIARCHIVE_BATTLECOMMANDS_HPP
#include "TargetT.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"

namespace open_viii::kernel {
template<LangT langVal> struct BattleCommands
{
  /*
   * https://github.com/alexfilth/doomtrain/wiki/Battle-commands
   * Offset	Length	Description
   * 	2 bytes	Offset to ability name
   * 	2 bytes	Offset to ability description
   * 	1 byte	Ability data ID
   * 	1 byte	Unknown Flags
   * 	1 byte	Target
   * 	1 byte	Unknown / Unused
   */
private:
  EncodedStringOffset m_name_offset{};
  EncodedStringOffset m_description_offset{};
  std::uint8_t m_ability_data_id{};
  std::uint8_t m_unknown_flags{};
  TargetT m_target{};
  std::uint8_t m_unknown{};


public:
  [[maybe_unused]] [[nodiscard]] const auto &name_offset() const noexcept
  {
    return m_name_offset;
  }
  [[maybe_unused]] [[nodiscard]] const auto &description_offset() const noexcept
  {
    return m_description_offset;
  }
  [[maybe_unused]] [[nodiscard]] const auto &ability_data_id() const noexcept
  {
    return m_ability_data_id;
  }
  [[maybe_unused]] [[nodiscard]] const auto &unknown_flags() const noexcept
  {
    return m_unknown_flags;
  }
  [[maybe_unused]] [[nodiscard]] const auto &get_target() const noexcept
  {
    return m_target;
  }
  [[nodiscard]] auto unknown() const noexcept
  {
    return m_unknown;
  }

  std::ostream &out(std::ostream &os, const std::span<const char> &buffer) const
  {
    auto name = m_name_offset.decoded_string<langVal>(buffer);
    auto description = m_description_offset.decoded_string<langVal>(buffer);
    if (!std::empty(name)) {
      os << tools::u8_to_sv(name);
    }
    if (!std::empty(description)) {
      os << ", " << tools::u8_to_sv(description);
    }
    return os

           << ", " << static_cast<std::uint32_t>(m_ability_data_id) << ", "
           << static_cast<std::uint32_t>(m_unknown_flags) << ", "
           << static_cast<std::uint32_t>(m_target) << ", "
           << static_cast<std::uint32_t>(m_unknown)

      ;
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_BATTLECOMMANDS_HPP
