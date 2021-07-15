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
#ifndef VIIIARCHIVE_CHARACTERS_HPP
#define VIIIARCHIVE_CHARACTERS_HPP
#include "CommonKernel.hpp"
#include "GenderT.hpp"
#include "StatGroupNoEVANoHIT.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
#include <compare>
namespace open_viii::kernel {
/**
 * Offset	Length	Description
 * 0x0000	2 bytes	Offset to character name
 * Squall and Rinoa have name offsets of 0xFFFF because their name is in the
 * save game data rather than kernel.bin.
 * 0x0002       1 byte	Crisis level hp multiplier
 * 0x0003	1 byte	Gender 0x00 - Male 0x01 - Female
 * 0x0004       1 byte  Limit Break ID
 * 0x0005	1 byte	Limit Break Param used for the power of
 * each renzokuken hit before finisher
 * 0x0006	2 bytes	EXP modifier
 * 0x0008	4 bytes	HP
 * 0x000C	4 bytes	STR
 * 0x0010	4 bytes	VIT
 * 0x0014	4 bytes	MAG
 * 0x0018	4 bytes	SPR
 * 0x001C	4 bytes	SPD
 * 0x0020	4 bytes	LUCK
 * @see https://github.com/DarkShinryu/doomtrain/wiki/Characters
 * */
struct Characters_impl
{
protected:
  EncodedStringOffset         m_name_offset                = {};
  std::uint8_t                m_crisis_level_hp_multiplier = {};
  GenderT                     m_gender                     = {};
  std::uint8_t                m_limit_break_id             = {};
  std::uint8_t                m_limit_break_param          = {};
  std::array<std::uint8_t, 2> m_exp                        = {};
  StatGroupNoEVANoHIT<std::array<std::uint8_t, 4>> m_stats = {};
  constexpr Characters_impl()                              = default;
  static constexpr std::size_t EXPECTED_SIZE               = 36U;

public:
  constexpr auto
    operator<=>(const Characters_impl &right) const noexcept = default;
};
using Characters = CommonKernel<Characters_impl>;
static_assert(sizeof(Characters) == Characters::EXPECTED_SIZE);
static_assert(has_name_offset<Characters>);
static_assert(has_crisis_level_hp_multiplier<Characters>);
static_assert(has_gender<Characters>);
static_assert(has_limit_break_id<Characters>);
static_assert(has_limit_break_param<Characters>);
static_assert(has_exp<Characters>);
static_assert(has_stats<Characters>);
static_assert(Characters().gender() == GenderT::male);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_CHARACTERS_HPP
