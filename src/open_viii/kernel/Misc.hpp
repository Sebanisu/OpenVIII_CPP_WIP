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
#ifndef VIIIARCHIVE_MISC_HPP
#define VIIIARCHIVE_MISC_HPP
#include "CommonKernel.hpp"
#include "CrisisLevelT.hpp"
#include "SequenceAndTimer.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
#include <compare>
namespace open_viii::kernel {
/**
 * Offset	Length	Description
 * 0x0000	1 byte	Sleep Timer
 * 0x0001	1 byte	Haste Timer
 * 0x0002	1 byte	Slow Timer
 * 0x0003	1 byte	Stop Timer
 * 0x0004	1 byte	Regen Timer
 * 0x0005	1 byte	Protect Timer
 * 0x0006	1 byte	Shell Timer
 * 0x0007	1 byte	Reflect Timer
 * 0x0008	1 byte	Aura Timer
 * 0x0009	1 byte	Curse Timer
 * 0x000A	1 byte	Doom Timer
 * 0x000B	1 byte	Invincible Timer
 * 0x000C	1 byte	Petrifying Timer
 * 0x000D	1 byte	Float Timer
 * 0x000E	1 byte	ATB Speed Multiplier
 * 0x000F	1 byte	Dead Timer
 * 0x0010	1 byte	Death Limit Effect
 * 0x0011	1 byte	Poison Limit Effect
 * 0x0012	1 byte	Petrify Limit Effect
 * 0x0013	1 byte	Darkness Limit Effect
 * 0x0014	1 byte	Silence Limit Effect
 * 0x0015	1 byte	Berserk Limit Effect
 * 0x0016	1 byte	Zombie Limit Effect
 * 0x0017	1 byte	Unknown Status Limit Effect
 * 0x0018	1 byte	Sleep Limit Effect
 * 0x0019	1 byte	Haste Limit Effect
 * 0x001A	1 byte	Slow Limit Effect
 * 0x001B	1 byte	Stop Limit Effect
 * 0x001C	1 byte	Regen Limit Effect
 * 0x001D	1 byte	Protect Limit Effect
 * 0x001E	1 byte	Shell Limit Effect
 * 0x001F	1 byte	Reflect Limit Effect
 * 0x0020	1 byte	Aura Limit Effect
 * 0x0021	1 byte	Curse Limit Effect
 * 0x0022	1 byte	Doom Limit Effect
 * 0x0023	1 byte	Invincible Limit Effect
 * 0x0024	1 byte	Petrifying Limit Effect
 * 0x0025	1 byte	Float Limit Effect
 * 0x0026	1 byte	Confusion Limit Effect
 * 0x0027	1 byte	Drain Limit Effect
 * 0x0028	1 byte	Eject Limit Effect
 * 0x0029	1 byte	Double Limit Effect
 * 0x002A	1 byte	Triple Limit Effect
 * 0x002B	1 byte	Defend Limit Effect
 * 0x002C	1 byte	Unknown Status Limit Effect
 * 0x002D	1 byte	Unknown Status Limit Effect
 * 0x002E	1 byte	Charged Limit Effect
 * 0x002F	1 byte	Back Attack Limit Effect
 * 0x0030	1 byte	Duel Start Sequence Crisis Level 1
 * 0x0031	1 byte	Duel Timer Crisis Level 1
 * 0x0032	1 byte	Duel Start Sequence Crisis Level 2
 * 0x0033	1 byte	Duel Timer Crisis Level 2
 * 0x0034	1 byte	Duel Start Sequence Crisis Level 3
 * 0x0035	1 byte	Duel Timer Crisis Level 3
 * 0x0036	1 byte	Duel Start Sequence Crisis Level 4
 * 0x0037	1 byte	Duel Timer Crisis Level 4
 * 0x0038	1 byte	Shot Timer Crisis Level 1
 * 0x0039	1 byte	Shot Timer Crisis Level 2
 * 0x003A	1 byte	Shot Timer Crisis Level 3
 * 0x003B	1 byte	Shot Timer Crisis Level 4
 * @see https://github.com/DarkShinryu/doomtrain/wiki/Misc-section
 */
struct Misc_impl
{
protected:
  std::uint8_t                   m_sleep_timer                  = {};
  std::uint8_t                   m_haste_timer                  = {};
  std::uint8_t                   m_slow_timer                   = {};
  std::uint8_t                   m_stop_timer                   = {};
  std::uint8_t                   m_regen_timer                  = {};
  std::uint8_t                   m_protect_timer                = {};
  std::uint8_t                   m_shell_timer                  = {};
  std::uint8_t                   m_reflect_timer                = {};
  std::uint8_t                   m_aura_timer                   = {};
  std::uint8_t                   m_curse_timer                  = {};
  std::uint8_t                   m_doom_timer                   = {};
  std::uint8_t                   m_invincible_timer             = {};
  std::uint8_t                   m_petrifying_timer             = {};
  std::uint8_t                   m_float_timer                  = {};
  std::uint8_t                   m_atb_speed_multiplier         = {};
  std::uint8_t                   m_dead_timer                   = {};
  std::uint8_t                   m_death_limit_effect           = {};
  std::uint8_t                   m_poison_limit_effect          = {};
  std::uint8_t                   m_petrify_limit_effect         = {};
  std::uint8_t                   m_darkness_limit_effect        = {};
  std::uint8_t                   m_silence_limit_effect         = {};
  std::uint8_t                   m_berserk_limit_effect         = {};
  std::uint8_t                   m_zombie_limit_effect          = {};
  std::uint8_t                   m_unknown_status_limit_effect  = {};
  std::uint8_t                   m_sleep_limit_effect           = {};
  std::uint8_t                   m_haste_limit_effect           = {};
  std::uint8_t                   m_slow_limit_effect            = {};
  std::uint8_t                   m_stop_limit_effect            = {};
  std::uint8_t                   m_regen_limit_effect           = {};
  std::uint8_t                   m_protect_limit_effect         = {};
  std::uint8_t                   m_shell_limit_effect           = {};
  std::uint8_t                   m_reflect_limit_effect         = {};
  std::uint8_t                   m_aura_limit_effect            = {};
  std::uint8_t                   m_curse_limit_effect           = {};
  std::uint8_t                   m_doom_limit_effect            = {};
  std::uint8_t                   m_invincible_limit_effect      = {};
  std::uint8_t                   m_petrifying_limit_effect      = {};
  std::uint8_t                   m_float_limit_effect           = {};
  std::uint8_t                   m_confusion_limit_effect       = {};
  std::uint8_t                   m_drain_limit_effect           = {};
  std::uint8_t                   m_eject_limit_effect           = {};
  std::uint8_t                   m_double_limit_effect          = {};
  std::uint8_t                   m_triple_limit_effect          = {};
  std::uint8_t                   m_defend_limit_effect          = {};
  std::uint8_t                   m_unknown_status_limit_effect0 = {};
  std::uint8_t                   m_unknown_status_limit_effect1 = {};
  std::uint8_t                   m_charged_limit_effect         = {};
  std::uint8_t                   m_back_attack_limit_effect     = {};
  CrisisLevelT<SequenceAndTimer> m_duel_timers                  = {};
  CrisisLevelT<std::uint8_t>     m_shot_timers                  = {};
  static constexpr std::size_t   EXPECTED_SIZE                  = 60U;
  constexpr Misc_impl()                                         = default;

public:
  constexpr auto
    operator<=>(const Misc_impl &right) const noexcept = default;
};
using Misc = CommonKernel<Misc_impl>;
static_assert(Misc::EXPECTED_SIZE == sizeof(Misc));
static_assert(has_sleep_timer<Misc>);
static_assert(has_haste_timer<Misc>);
static_assert(has_slow_timer<Misc>);
static_assert(has_stop_timer<Misc>);
static_assert(has_regen_timer<Misc>);
static_assert(has_protect_timer<Misc>);
static_assert(has_shell_timer<Misc>);
static_assert(has_reflect_timer<Misc>);
static_assert(has_aura_timer<Misc>);
static_assert(has_curse_timer<Misc>);
static_assert(has_doom_timer<Misc>);
static_assert(has_invincible_timer<Misc>);
static_assert(has_petrifying_timer<Misc>);
static_assert(has_float_timer<Misc>);
static_assert(has_atb_speed_multiplier<Misc>);
static_assert(has_dead_timer<Misc>);
static_assert(has_death_limit_effect<Misc>);
static_assert(has_poison_limit_effect<Misc>);
static_assert(has_petrify_limit_effect<Misc>);
static_assert(has_darkness_limit_effect<Misc>);
static_assert(has_silence_limit_effect<Misc>);
static_assert(has_berserk_limit_effect<Misc>);
static_assert(has_zombie_limit_effect<Misc>);
static_assert(has_unknown_status_limit_effect<Misc>);
static_assert(has_sleep_limit_effect<Misc>);
static_assert(has_haste_limit_effect<Misc>);
static_assert(has_slow_limit_effect<Misc>);
static_assert(has_stop_limit_effect<Misc>);
static_assert(has_regen_limit_effect<Misc>);
static_assert(has_protect_limit_effect<Misc>);
static_assert(has_shell_limit_effect<Misc>);
static_assert(has_reflect_limit_effect<Misc>);
static_assert(has_aura_limit_effect<Misc>);
static_assert(has_curse_limit_effect<Misc>);
static_assert(has_doom_limit_effect<Misc>);
static_assert(has_invincible_limit_effect<Misc>);
static_assert(has_petrifying_limit_effect<Misc>);
static_assert(has_float_limit_effect<Misc>);
static_assert(has_confusion_limit_effect<Misc>);
static_assert(has_drain_limit_effect<Misc>);
static_assert(has_eject_limit_effect<Misc>);
static_assert(has_double_limit_effect<Misc>);
static_assert(has_triple_limit_effect<Misc>);
static_assert(has_defend_limit_effect<Misc>);
static_assert(has_unknown_status_limit_effect0<Misc>);
static_assert(has_unknown_status_limit_effect1<Misc>);
static_assert(has_charged_limit_effect<Misc>);
static_assert(has_back_attack_limit_effect<Misc>);
static_assert(has_duel_timers<Misc>);
static_assert(has_shot_timers<Misc>);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_MISC_HPP
