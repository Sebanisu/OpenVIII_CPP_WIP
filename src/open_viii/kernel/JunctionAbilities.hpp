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
#ifndef VIIIARCHIVE_JUNCTIONABILITIES_HPP
#define VIIIARCHIVE_JUNCTIONABILITIES_HPP
#include "CommonKernel.hpp"
#include "JunctionFlagsT.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
#include <compare>
#include <cstring>
namespace open_viii::kernel {
struct JunctionAbilities_impl
{
  /**
   * https://github.com/DarkShinryu/doomtrain/wiki/Junction-abilities
   * 0x0000	2 bytes	Offset to ability name
   * 0x0002	2 bytes	Offset to ability description
   * 0x0004	1 byte	AP Required to learn ability
   * 0x0005	3 byte	J-Flag
   */
protected:
  EncodedStringOffset m_name_offset                       = {};
  EncodedStringOffset m_description_offset                = {};
  std::uint8_t        m_ability_points_required_to_unlock = {};
  constexpr JunctionAbilities_impl()                      = default;
  static constexpr std::size_t EXPECTED_SIZE              = 8U;
  [[nodiscard]] constexpr JunctionFlagsT
  junction_flags_impl() const
  {
    return static_cast<JunctionFlagsT>(
      (m_junction_flags0 << 16U) | (m_junction_flags1 << 8U)
      | m_junction_flags2);
  }

private:
  std::uint8_t m_junction_flags0 = {};
  std::uint8_t m_junction_flags1 = {};
  std::uint8_t m_junction_flags2 = {};

public:
  constexpr auto
    operator<=>(const JunctionAbilities_impl &right) const noexcept = default;
};
using JunctionAbilities = CommonKernel<JunctionAbilities_impl>;
static_assert(JunctionAbilities::EXPECTED_SIZE == sizeof(JunctionAbilities));
static_assert(has_name_offset<JunctionAbilities>);
static_assert(has_description_offset<JunctionAbilities>);
static_assert(has_ability_points_required_to_unlock<JunctionAbilities>);
static_assert(has_junction_flags<JunctionAbilities>);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_JUNCTIONABILITIES_HPP
