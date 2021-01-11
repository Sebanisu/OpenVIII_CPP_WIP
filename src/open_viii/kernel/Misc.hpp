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
#include "CrisisLevelT.hpp"
#include "SequenceAndTimer.hpp"
#include "open_viii/Strings/EncodedStringOffset.hpp"
namespace open_viii::kernel {
template<LangT langVal> struct Misc
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Misc-section
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
   */


private:
  std::uint8_t m_sleep_timer{};
  std::uint8_t m_haste_timer{};
  std::uint8_t m_slow_timer{};
  std::uint8_t m_stop_timer{};
  std::uint8_t m_regen_timer{};
  std::uint8_t m_protect_timer{};
  std::uint8_t m_shell_timer{};
  std::uint8_t m_reflect_timer{};
  std::uint8_t m_aura_timer{};
  std::uint8_t m_curse_timer{};
  std::uint8_t m_doom_timer{};
  std::uint8_t m_invincible_timer{};
  std::uint8_t m_petrifying_timer{};
  std::uint8_t m_float_timer{};
  std::uint8_t m_atb_speed_multiplier{};
  std::uint8_t m_dead_timer{};
  std::uint8_t m_death_limit_effect{};
  std::uint8_t m_poison_limit_effect{};
  std::uint8_t m_petrify_limit_effect{};
  std::uint8_t m_darkness_limit_effect{};
  std::uint8_t m_silence_limit_effect{};
  std::uint8_t m_berserk_limit_effect{};
  std::uint8_t m_zombie_limit_effect{};
  std::uint8_t m_unknown_status_limit_effect{};
  std::uint8_t m_sleep_limit_effect{};
  std::uint8_t m_haste_limit_effect{};
  std::uint8_t m_slow_limit_effect{};
  std::uint8_t m_stop_limit_effect{};
  std::uint8_t m_regen_limit_effect{};
  std::uint8_t m_protect_limit_effect{};
  std::uint8_t m_shell_limit_effect{};
  std::uint8_t m_reflect_limit_effect{};
  std::uint8_t m_aura_limit_effect{};
  std::uint8_t m_curse_limit_effect{};
  std::uint8_t m_doom_limit_effect{};
  std::uint8_t m_invincible_limit_effect{};
  std::uint8_t m_petrifying_limit_effect{};
  std::uint8_t m_float_limit_effect{};
  std::uint8_t m_confusion_limit_effect{};
  std::uint8_t m_drain_limit_effect{};
  std::uint8_t m_eject_limit_effect{};
  std::uint8_t m_double_limit_effect{};
  std::uint8_t m_triple_limit_effect{};
  std::uint8_t m_defend_limit_effect{};
  std::uint8_t m_unknown_status_limit_effect0{};
  std::uint8_t m_unknown_status_limit_effect1{};
  std::uint8_t m_charged_limit_effect{};
  std::uint8_t m_back_attack_limit_effect{};
  CrisisLevelT<SequenceAndTimer> m_duel_timers{};
  CrisisLevelT<std::uint8_t> m_shot_timers{};

public:
  [[maybe_unused]] [[nodiscard]] auto sleep_timer() const noexcept
  {
    return m_sleep_timer;
  }
  [[maybe_unused]] [[nodiscard]] auto haste_timer() const noexcept
  {
    return m_haste_timer;
  }
  [[maybe_unused]] [[nodiscard]] auto slow_timer() const noexcept
  {
    return m_slow_timer;
  }
  [[maybe_unused]] [[nodiscard]] auto stop_timer() const noexcept
  {
    return m_stop_timer;
  }
  [[maybe_unused]] [[nodiscard]] auto regen_timer() const noexcept
  {
    return m_regen_timer;
  }
  [[maybe_unused]] [[nodiscard]] auto protect_timer() const noexcept
  {
    return m_protect_timer;
  }
  [[maybe_unused]] [[nodiscard]] auto shell_timer() const noexcept
  {
    return m_shell_timer;
  }
  [[maybe_unused]] [[nodiscard]] auto reflect_timer() const noexcept
  {
    return m_reflect_timer;
  }
  [[maybe_unused]] [[nodiscard]] auto aura_timer() const noexcept
  {
    return m_aura_timer;
  }
  [[maybe_unused]] [[nodiscard]] auto curse_timer() const noexcept
  {
    return m_curse_timer;
  }
  [[maybe_unused]] [[nodiscard]] auto doom_timer() const noexcept
  {
    return m_doom_timer;
  }
  [[maybe_unused]] [[nodiscard]] auto invincible_timer() const noexcept
  {
    return m_invincible_timer;
  }
  [[maybe_unused]] [[nodiscard]] auto petrifying_timer() const noexcept
  {
    return m_petrifying_timer;
  }
  [[maybe_unused]] [[nodiscard]] auto float_timer() const noexcept
  {
    return m_float_timer;
  }
  [[maybe_unused]] [[nodiscard]] auto atb_speed_multiplier() const noexcept
  {
    return m_atb_speed_multiplier;
  }
  [[maybe_unused]] [[nodiscard]] auto dead_timer() const noexcept
  {
    return m_dead_timer;
  }
  [[nodiscard]] auto death_limit_effect() const noexcept
  {
    return m_death_limit_effect;
  }
  [[nodiscard]] auto poison_limit_effect() const noexcept
  {
    return m_poison_limit_effect;
  }
  [[maybe_unused]] [[nodiscard]] auto petrify_limit_effect() const noexcept
  {
    return m_petrify_limit_effect;
  }
  [[maybe_unused]] [[nodiscard]] auto darkness_limit_effect() const noexcept
  {
    return m_darkness_limit_effect;
  }
  [[nodiscard]] auto silence_limit_effect() const noexcept
  {
    return m_silence_limit_effect;
  }
  [[nodiscard]] auto berserk_limit_effect() const noexcept
  {
    return m_berserk_limit_effect;
  }
  [[maybe_unused]] [[nodiscard]] auto zombie_limit_effect() const noexcept
  {
    return m_zombie_limit_effect;
  }
  [[maybe_unused]] [[nodiscard]] auto
    unknown_status_limit_effect() const noexcept
  {
    return m_unknown_status_limit_effect;
  }
  [[maybe_unused]] [[nodiscard]] auto sleep_limit_effect() const noexcept
  {
    return m_sleep_limit_effect;
  }
  [[maybe_unused]] [[nodiscard]] auto haste_limit_effect() const noexcept
  {
    return m_haste_limit_effect;
  }
  [[maybe_unused]] [[nodiscard]] auto slow_limit_effect() const noexcept
  {
    return m_slow_limit_effect;
  }
  [[maybe_unused]] [[nodiscard]] auto stop_limit_effect() const noexcept
  {
    return m_stop_limit_effect;
  }
  [[nodiscard]] auto regen_limit_effect() const noexcept
  {
    return m_regen_limit_effect;
  }
  [[maybe_unused]] [[nodiscard]] auto protect_limit_effect() const noexcept
  {
    return m_protect_limit_effect;
  }
  [[maybe_unused]] [[nodiscard]] auto shell_limit_effect() const noexcept
  {
    return m_shell_limit_effect;
  }
  [[nodiscard]] auto reflect_limit_effect() const noexcept
  {
    return m_reflect_limit_effect;
  }
  [[maybe_unused]] [[nodiscard]] auto aura_limit_effect() const noexcept
  {
    return m_aura_limit_effect;
  }
  [[maybe_unused]] [[nodiscard]] auto curse_limit_effect() const noexcept
  {
    return m_curse_limit_effect;
  }
  [[maybe_unused]] [[nodiscard]] auto doom_limit_effect() const noexcept
  {
    return m_doom_limit_effect;
  }
  [[maybe_unused]] [[nodiscard]] auto invincible_limit_effect() const noexcept
  {
    return m_invincible_limit_effect;
  }
  [[maybe_unused]] [[nodiscard]] auto petrifying_limit_effect() const noexcept
  {
    return m_petrifying_limit_effect;
  }
  [[maybe_unused]] [[nodiscard]] auto float_limit_effect() const noexcept
  {
    return m_float_limit_effect;
  }
  [[nodiscard]] auto confusion_limit_effect() const noexcept
  {
    return m_confusion_limit_effect;
  }
  [[nodiscard]] auto drain_limit_effect() const noexcept
  {
    return m_drain_limit_effect;
  }
  [[maybe_unused]] [[nodiscard]] auto eject_limit_effect() const noexcept
  {
    return m_eject_limit_effect;
  }
  [[maybe_unused]] [[nodiscard]] auto double_limit_effect() const noexcept
  {
    return m_double_limit_effect;
  }
  [[maybe_unused]] [[nodiscard]] auto triple_limit_effect() const noexcept
  {
    return m_triple_limit_effect;
  }
  [[maybe_unused]] [[nodiscard]] auto defend_limit_effect() const noexcept
  {
    return m_defend_limit_effect;
  }
  [[maybe_unused]] [[nodiscard]] auto
    unknown_status_limit_effect0() const noexcept
  {
    return m_unknown_status_limit_effect0;
  }
  [[maybe_unused]] [[nodiscard]] auto
    unknown_status_limit_effect1() const noexcept
  {
    return m_unknown_status_limit_effect1;
  }
  [[maybe_unused]] [[nodiscard]] auto charged_limit_effect() const noexcept
  {
    return m_charged_limit_effect;
  }
  [[nodiscard]] auto back_attack_limit_effect() const noexcept
  {
    return m_back_attack_limit_effect;
  }
  [[maybe_unused]] [[nodiscard]] auto duel_timers() const noexcept
  {
    return m_duel_timers;
  }
  [[maybe_unused]] [[nodiscard]] auto shot_timers() const noexcept
  {
    return m_shot_timers;
  }
  std::ostream &out(std::ostream &os,
    [[maybe_unused]] const std::span<const char> &buffer) const
  {
    return os << static_cast<std::uint32_t>(m_sleep_timer) << ", "
              << static_cast<std::uint32_t>(m_haste_timer) << ", "
              << static_cast<std::uint32_t>(m_slow_timer) << ", "
              << static_cast<std::uint32_t>(m_stop_timer) << ", "
              << static_cast<std::uint32_t>(m_regen_timer) << ", "
              << static_cast<std::uint32_t>(m_protect_timer) << ", "
              << static_cast<std::uint32_t>(m_shell_timer) << ", "
              << static_cast<std::uint32_t>(m_reflect_timer) << ", "
              << static_cast<std::uint32_t>(m_aura_timer) << ", "
              << static_cast<std::uint32_t>(m_curse_timer) << ", "
              << static_cast<std::uint32_t>(m_doom_timer) << ", "
              << static_cast<std::uint32_t>(m_invincible_timer) << ", "
              << static_cast<std::uint32_t>(m_petrifying_timer) << ", "
              << static_cast<std::uint32_t>(m_float_timer) << ", "
              << static_cast<std::uint32_t>(m_atb_speed_multiplier) << ", "
              << static_cast<std::uint32_t>(m_dead_timer) << ", "
              << static_cast<std::uint32_t>(m_death_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_poison_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_petrify_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_darkness_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_silence_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_berserk_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_zombie_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_unknown_status_limit_effect)
              << ", " << static_cast<std::uint32_t>(m_sleep_limit_effect)
              << ", " << static_cast<std::uint32_t>(m_haste_limit_effect)
              << ", " << static_cast<std::uint32_t>(m_slow_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_stop_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_regen_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_protect_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_shell_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_reflect_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_aura_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_curse_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_doom_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_invincible_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_petrifying_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_float_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_confusion_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_drain_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_eject_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_double_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_triple_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_defend_limit_effect) << ", "
              << static_cast<std::uint32_t>(m_unknown_status_limit_effect0)
              << ", "
              << static_cast<std::uint32_t>(m_unknown_status_limit_effect1)
              << ", " << static_cast<std::uint32_t>(m_charged_limit_effect)
              << ", " << static_cast<std::uint32_t>(m_back_attack_limit_effect)
              << ", " << m_duel_timers << ", " << m_shot_timers;
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_MISC_HPP
