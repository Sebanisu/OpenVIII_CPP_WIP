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

#ifndef VIIIARCHIVE_MISC_H
#define VIIIARCHIVE_MISC_H
#include "OpenVIII/Strings/EncodedStringOffset.h"
#include "SequenceAndTimer.h"
#include "CrisisLevelT.h"
namespace OpenVIII::Kernel { template <LangT langVal> struct Misc
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
  std::uint8_t sleepTimer_{};
  std::uint8_t hasteTimer_{};
  std::uint8_t slowTimer_{};
  std::uint8_t stopTimer_{};
  std::uint8_t regenTimer_{};
  std::uint8_t protectTimer_{};
  std::uint8_t shellTimer_{};
  std::uint8_t reflectTimer_{};
  std::uint8_t auraTimer_{};
  std::uint8_t curseTimer_{};
  std::uint8_t doomTimer_{};
  std::uint8_t invincibleTimer_{};
  std::uint8_t petrifyingTimer_{};
  std::uint8_t floatTimer_{};
  std::uint8_t atbSpeedMultiplier_{};
  std::uint8_t deadTimer_{};
  std::uint8_t deathLimitEffect_{};
  std::uint8_t poisonLimitEffect_{};
  std::uint8_t petrifyLimitEffect_{};
  std::uint8_t darknessLimitEffect_{};
  std::uint8_t silenceLimitEffect_{};
  std::uint8_t berserkLimitEffect_{};
  std::uint8_t zombieLimitEffect_{};
  std::uint8_t unknownStatusLimitEffect_{};
  std::uint8_t sleepLimitEffect_{};
  std::uint8_t hasteLimitEffect_{};
  std::uint8_t slowLimitEffect_{};
  std::uint8_t stopLimitEffect_{};
  std::uint8_t regenLimitEffect_{};
  std::uint8_t protectLimitEffect_{};
  std::uint8_t shellLimitEffect_{};
  std::uint8_t reflectLimitEffect_{};
  std::uint8_t auraLimitEffect_{};
  std::uint8_t curseLimitEffect_{};
  std::uint8_t doomLimitEffect_{};
  std::uint8_t invincibleLimitEffect_{};
  std::uint8_t petrifyingLimitEffect_{};
  std::uint8_t floatLimitEffect_{};
  std::uint8_t confusionLimitEffect_{};
  std::uint8_t drainLimitEffect_{};
  std::uint8_t ejectLimitEffect_{};
  std::uint8_t doubleLimitEffect_{};
  std::uint8_t tripleLimitEffect_{};
  std::uint8_t defendLimitEffect_{};
  std::uint8_t unknownStatusLimitEffect0_{};
  std::uint8_t UnknownStatusLimitEffect1_{};
  std::uint8_t chargedLimitEffect_{};
  std::uint8_t backAttackLimitEffect_{};
  CrisisLevelT<SequenceAndTimer> duelTimers_{};
  CrisisLevelT<std::uint8_t> shotTimers_{};

public:
  [[nodiscard]] auto SleepTimer() const noexcept { return sleepTimer_; }
  [[nodiscard]] auto HasteTimer() const noexcept { return hasteTimer_; }
  [[nodiscard]] auto SlowTimer() const noexcept { return slowTimer_; }
  [[nodiscard]] auto StopTimer() const noexcept { return stopTimer_; }
  [[nodiscard]] auto RegenTimer() const noexcept { return regenTimer_; }
  [[nodiscard]] auto ProtectTimer() const noexcept { return protectTimer_; }
  [[nodiscard]] auto ShellTimer() const noexcept { return shellTimer_; }
  [[nodiscard]] auto ReflectTimer() const noexcept { return reflectTimer_; }
  [[nodiscard]] auto AuraTimer() const noexcept { return auraTimer_; }
  [[nodiscard]] auto CurseTimer() const noexcept { return curseTimer_; }
  [[nodiscard]] auto DoomTimer() const noexcept { return doomTimer_; }
  [[nodiscard]] auto InvincibleTimer() const noexcept { return invincibleTimer_; }
  [[nodiscard]] auto PetrifyingTimer() const noexcept { return petrifyingTimer_; }
  [[nodiscard]] auto FloatTimer() const noexcept { return floatTimer_; }
  [[nodiscard]] auto ATBSpeedMultiplier() const noexcept { return atbSpeedMultiplier_; }
  [[nodiscard]] auto DeadTimer() const noexcept { return deadTimer_; }
  [[nodiscard]] auto DeathLimitEffect() const noexcept { return deathLimitEffect_; }
  [[nodiscard]] auto PoisonLimitEffect() const noexcept { return poisonLimitEffect_; }
  [[nodiscard]] auto PetrifyLimitEffect() const noexcept { return petrifyLimitEffect_; }
  [[nodiscard]] auto DarknessLimitEffect() const noexcept { return darknessLimitEffect_; }
  [[nodiscard]] auto SilenceLimitEffect() const noexcept { return silenceLimitEffect_; }
  [[nodiscard]] auto BerserkLimitEffect() const noexcept { return berserkLimitEffect_; }
  [[nodiscard]] auto ZombieLimitEffect() const noexcept { return zombieLimitEffect_; }
  [[nodiscard]] auto UnknownStatusLimitEffect() const noexcept { return unknownStatusLimitEffect_; }
  [[nodiscard]] auto SleepLimitEffect() const noexcept { return sleepLimitEffect_; }
  [[nodiscard]] auto HasteLimitEffect() const noexcept { return hasteLimitEffect_; }
  [[nodiscard]] auto SlowLimitEffect() const noexcept { return slowLimitEffect_; }
  [[nodiscard]] auto StopLimitEffect() const noexcept { return stopLimitEffect_; }
  [[nodiscard]] auto RegenLimitEffect() const noexcept { return regenLimitEffect_; }
  [[nodiscard]] auto ProtectLimitEffect() const noexcept { return protectLimitEffect_; }
  [[nodiscard]] auto ShellLimitEffect() const noexcept { return shellLimitEffect_; }
  [[nodiscard]] auto ReflectLimitEffect() const noexcept { return reflectLimitEffect_; }
  [[nodiscard]] auto AuraLimitEffect() const noexcept { return auraLimitEffect_; }
  [[nodiscard]] auto CurseLimitEffect() const noexcept { return curseLimitEffect_; }
  [[nodiscard]] auto DoomLimitEffect() const noexcept { return doomLimitEffect_; }
  [[nodiscard]] auto InvincibleLimitEffect() const noexcept { return invincibleLimitEffect_; }
  [[nodiscard]] auto PetrifyingLimitEffect() const noexcept { return petrifyingLimitEffect_; }
  [[nodiscard]] auto FloatLimitEffect() const noexcept { return floatLimitEffect_; }
  [[nodiscard]] auto ConfusionLimitEffect() const noexcept { return confusionLimitEffect_; }
  [[nodiscard]] auto DrainLimitEffect() const noexcept { return drainLimitEffect_; }
  [[nodiscard]] auto EjectLimitEffect() const noexcept { return ejectLimitEffect_; }
  [[nodiscard]] auto DoubleLimitEffect() const noexcept { return doubleLimitEffect_; }
  [[nodiscard]] auto TripleLimitEffect() const noexcept { return tripleLimitEffect_; }
  [[nodiscard]] auto DefendLimitEffect() const noexcept { return defendLimitEffect_; }
  [[nodiscard]] auto UnknownStatusLimitEffect0() const noexcept { return unknownStatusLimitEffect0_; }
  [[nodiscard]] auto UnknownStatusLimitEffect1() const noexcept { return UnknownStatusLimitEffect1_; }
  [[nodiscard]] auto ChargedLimitEffect() const noexcept { return chargedLimitEffect_; }
  [[nodiscard]] auto BackAttackLimitEffect() const noexcept { return backAttackLimitEffect_; }
  [[nodiscard]] auto DuelTimers() const noexcept { return duelTimers_; }
  [[nodiscard]] auto ShotTimers() const noexcept { return shotTimers_; }
  std::ostream &Out(std::ostream &os, [[maybe_unused]] const std::string_view &buffer) const
  {
    return os << static_cast<std::uint32_t>(SleepTimer()) << ", " << static_cast<std::uint32_t>(HasteTimer()) << ", "
              << static_cast<std::uint32_t>(SlowTimer()) << ", " << static_cast<std::uint32_t>(StopTimer()) << ", "
              << static_cast<std::uint32_t>(RegenTimer()) << ", " << static_cast<std::uint32_t>(ProtectTimer()) << ", "
              << static_cast<std::uint32_t>(ShellTimer()) << ", " << static_cast<std::uint32_t>(ReflectTimer()) << ", "
              << static_cast<std::uint32_t>(AuraTimer()) << ", " << static_cast<std::uint32_t>(CurseTimer()) << ", "
              << static_cast<std::uint32_t>(DoomTimer()) << ", " << static_cast<std::uint32_t>(InvincibleTimer())
              << ", " << static_cast<std::uint32_t>(PetrifyingTimer()) << ", "
              << static_cast<std::uint32_t>(FloatTimer()) << ", " << static_cast<std::uint32_t>(ATBSpeedMultiplier())
              << ", " << static_cast<std::uint32_t>(DeadTimer()) << ", "
              << static_cast<std::uint32_t>(DeathLimitEffect()) << ", "
              << static_cast<std::uint32_t>(PoisonLimitEffect()) << ", "
              << static_cast<std::uint32_t>(PetrifyLimitEffect()) << ", "
              << static_cast<std::uint32_t>(DarknessLimitEffect()) << ", "
              << static_cast<std::uint32_t>(SilenceLimitEffect()) << ", "
              << static_cast<std::uint32_t>(BerserkLimitEffect()) << ", "
              << static_cast<std::uint32_t>(ZombieLimitEffect()) << ", "
              << static_cast<std::uint32_t>(UnknownStatusLimitEffect()) << ", "
              << static_cast<std::uint32_t>(SleepLimitEffect()) << ", "
              << static_cast<std::uint32_t>(HasteLimitEffect()) << ", " << static_cast<std::uint32_t>(SlowLimitEffect())
              << ", " << static_cast<std::uint32_t>(StopLimitEffect()) << ", "
              << static_cast<std::uint32_t>(RegenLimitEffect()) << ", "
              << static_cast<std::uint32_t>(ProtectLimitEffect()) << ", "
              << static_cast<std::uint32_t>(ShellLimitEffect()) << ", "
              << static_cast<std::uint32_t>(ReflectLimitEffect()) << ", "
              << static_cast<std::uint32_t>(AuraLimitEffect()) << ", " << static_cast<std::uint32_t>(CurseLimitEffect())
              << ", " << static_cast<std::uint32_t>(DoomLimitEffect()) << ", "
              << static_cast<std::uint32_t>(InvincibleLimitEffect()) << ", "
              << static_cast<std::uint32_t>(PetrifyingLimitEffect()) << ", "
              << static_cast<std::uint32_t>(FloatLimitEffect()) << ", "
              << static_cast<std::uint32_t>(ConfusionLimitEffect()) << ", "
              << static_cast<std::uint32_t>(DrainLimitEffect()) << ", "
              << static_cast<std::uint32_t>(EjectLimitEffect()) << ", "
              << static_cast<std::uint32_t>(DoubleLimitEffect()) << ", "
              << static_cast<std::uint32_t>(TripleLimitEffect()) << ", "
              << static_cast<std::uint32_t>(DefendLimitEffect()) << ", "
              << static_cast<std::uint32_t>(UnknownStatusLimitEffect0()) << ", "
              << static_cast<std::uint32_t>(UnknownStatusLimitEffect1()) << ", "
              << static_cast<std::uint32_t>(ChargedLimitEffect()) << ", "
              << static_cast<std::uint32_t>(BackAttackLimitEffect()) << ", " << DuelTimers() << ", " << ShotTimers();
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_MISC_H
