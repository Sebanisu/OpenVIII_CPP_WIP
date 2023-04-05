//
// Created by pcvii on 4/3/2023.
//

#ifndef OPENVIII_CPP_WIP_MENTALRESIST_HPP
#define OPENVIII_CPP_WIP_MENTALRESIST_HPP
#include "open_viii/kernel/BattleOnlyStatusesT.hpp"
#include "open_viii/kernel/PersistentStatusesT.hpp"
#include <cstdint>
#include <stdexcept>
namespace open_viii::battle::dat {
/**
 * @brief Represents mental resistances for a character.
 */
struct MentalResist
{
  std::uint8_t Death;      ///< Death resistance.
  std::uint8_t Poison;     ///< Poison resistance.
  std::uint8_t Petrify;    ///< Petrify resistance.
  std::uint8_t Darkness;   ///< Darkness resistance.
  std::uint8_t Silence;    ///< Silence resistance.
  std::uint8_t Berserk;    ///< Berserk resistance
  std::uint8_t Zombie;     ///< Zombie resistance
  std::uint8_t Sleep;      ///< Sleep resistance
  std::uint8_t Haste;      ///< Haste resistance
  std::uint8_t Slow;       ///< Slow resistance
  std::uint8_t Stop;       ///< Stop resistance
  std::uint8_t Regen;      ///< Regen resistance
  std::uint8_t Reflect;    ///< Reflect resistance
  std::uint8_t Doom;       ///< Doom resistance
  std::uint8_t SlowPetrify;///< SlowPetrify resistance
  std::uint8_t Float;      ///< Float resistance
  std::uint8_t Confuse;    ///< Confuse resistance
  std::uint8_t Drain;      ///< Drain resistance
  std::uint8_t Expulsion;  ///< Expulsion resistance
  std::uint8_t Percent;    ///< Percent resistance

  std::uint8_t &
    operator[](kernel::PersistentStatusesT status)
  {
    using kernel::PersistentStatusesT;
    switch (status) {
    case PersistentStatusesT::death:
      return Death;
    case PersistentStatusesT::poison:
      return Poison;
    case PersistentStatusesT::petrify:
      return Petrify;
    case PersistentStatusesT::darkness:
      return Darkness;
    case PersistentStatusesT::silence:
      return Silence;
    case PersistentStatusesT::berserk:
      return Berserk;
    case PersistentStatusesT::zombie:
      return Zombie;
    // ... (Other cases)
    default:
      throw std::invalid_argument("Invalid status");
    }
  }

  std::uint8_t &
    operator[](kernel::BattleOnlyStatusesT status)
  {
    using kernel::BattleOnlyStatusesT;
    switch (status) {
    case BattleOnlyStatusesT::sleep:
      return Sleep;
    case BattleOnlyStatusesT::haste:
      return Haste;
    case BattleOnlyStatusesT::slow:
      return Slow;
    case BattleOnlyStatusesT::stop:
      return Stop;
    case BattleOnlyStatusesT::regen:
      return Regen;
    case BattleOnlyStatusesT::reflect:
      return Reflect;
    case BattleOnlyStatusesT::doom:
      return Doom;
    case BattleOnlyStatusesT::petrifying:
      return SlowPetrify;
    case BattleOnlyStatusesT::floating:
      return Float;
    case BattleOnlyStatusesT::confuse:
      return Confuse;
    case BattleOnlyStatusesT::eject:
      return Expulsion;
    // case BattleOnlyStatusesT::???: return Percent;
    //  ... (Other cases)
    default:
      throw std::invalid_argument("Invalid status");
    }
  }

  const std::uint8_t &
    operator[](kernel::PersistentStatusesT status) const
  {
    using kernel::PersistentStatusesT;
    switch (status) {
    case PersistentStatusesT::death:
      return Death;
    case PersistentStatusesT::poison:
      return Poison;
    case PersistentStatusesT::petrify:
      return Petrify;
    case PersistentStatusesT::darkness:
      return Darkness;
    case PersistentStatusesT::silence:
      return Silence;
    case PersistentStatusesT::berserk:
      return Berserk;
    case PersistentStatusesT::zombie:
      return Zombie;
    // ... (Other cases)
    default:
      throw std::invalid_argument("Invalid status");
    }
  }

  const std::uint8_t &
    operator[](kernel::BattleOnlyStatusesT status) const
  {
    using kernel::BattleOnlyStatusesT;
    switch (status) {
    case BattleOnlyStatusesT::sleep:
      return Sleep;
    case BattleOnlyStatusesT::haste:
      return Haste;
    case BattleOnlyStatusesT::slow:
      return Slow;
    case BattleOnlyStatusesT::stop:
      return Stop;
    case BattleOnlyStatusesT::regen:
      return Regen;
    case BattleOnlyStatusesT::reflect:
      return Reflect;
    case BattleOnlyStatusesT::doom:
      return Doom;
    case BattleOnlyStatusesT::petrifying:
      return SlowPetrify;
    case BattleOnlyStatusesT::floating:
      return Float;
    case BattleOnlyStatusesT::confuse:
      return Confuse;
    case BattleOnlyStatusesT::eject:
      return Expulsion;
    // case BattleOnlyStatusesT::???: return Percent;
    //  ... (Other cases)
    default:
      throw std::invalid_argument("Invalid status");
    }
  }
};
}// namespace open_viii::battle
#endif// OPENVIII_CPP_WIP_MENTALRESIST_HPP
