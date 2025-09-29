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
#ifndef VIIIARCHIVE_QUISTISBLUEMAGICLIMITBREAK_HPP
#define VIIIARCHIVE_QUISTISBLUEMAGICLIMITBREAK_HPP
#include "AttackFlagsT.hpp"
#include "AttackTypeT.hpp"
#include "CommonKernel.hpp"
#include "ElementT.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
namespace open_viii::kernel {
/**
 * 0x0000	2 bytes	Offset to limit name
 * 0x0002	2 bytes	Offset to limit description
 * 0x0004	2 bytes	Magic ID
 * 0x0006	1 byte	Unknown
 * 0x0007	1 byte	Attack Type
 * 0x0008	2 bytes	Unknown
 * 0x000A	1 byte	Attack Flags
 * 0x000B	1 byte	Unknown
 * 0x000C	1 byte	Element
 * 0x000D	1 byte	Status Attack
 * 0x000E	1 byte	Critical Bonus
 * 0x000F	1 byte	Unknown
 * @see
 * https://github.com/DarkShinryu/doomtrain/wiki/Blue-magic-(Quistis-limit-break)
 */
struct QuistisBlueMagicLimitBreak_impl
{
protected:
  EncodedStringOffset          m_name_offset        = {};
  EncodedStringOffset          m_description_offset = {};
  std::uint16_t                m_magic_id           = {};
  std::uint8_t                 m_unknown0           = {};
  AttackTypeT                  m_attack_type        = {};
  std::uint8_t                 m_unknown1           = {};
  std::uint8_t                 m_unknown2           = {};
  AttackFlagsT                 m_attack_flags       = {};
  std::uint8_t                 m_unknown3           = {};
  ElementT                     m_element            = {};
  std::uint8_t                 m_status_attack      = {};
  std::uint8_t                 m_critical_bonus     = {};
  std::uint8_t                 m_unknown4           = {};
  static constexpr std::size_t EXPECTED_SIZE        = 16U;
  constexpr QuistisBlueMagicLimitBreak_impl()       = default;

public:
  constexpr auto
    operator<=>(const QuistisBlueMagicLimitBreak_impl &right) const noexcept
    = default;
};
using QuistisBlueMagicLimitBreak
  = CommonKernel<QuistisBlueMagicLimitBreak_impl>;
static_assert(has_name_offset<QuistisBlueMagicLimitBreak>);
static_assert(has_description_offset<QuistisBlueMagicLimitBreak>);
static_assert(has_magic_id<QuistisBlueMagicLimitBreak>);
static_assert(has_unknown0<QuistisBlueMagicLimitBreak>);
static_assert(has_attack_type<QuistisBlueMagicLimitBreak>);
static_assert(has_unknown1<QuistisBlueMagicLimitBreak>);
static_assert(has_unknown2<QuistisBlueMagicLimitBreak>);
static_assert(has_attack_flags<QuistisBlueMagicLimitBreak>);
static_assert(has_unknown3<QuistisBlueMagicLimitBreak>);
static_assert(has_element<QuistisBlueMagicLimitBreak>);
static_assert(has_status_attack<QuistisBlueMagicLimitBreak>);
static_assert(has_critical_bonus<QuistisBlueMagicLimitBreak>);
static_assert(has_unknown4<QuistisBlueMagicLimitBreak>);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_QUISTISBLUEMAGICLIMITBREAK_HPP
