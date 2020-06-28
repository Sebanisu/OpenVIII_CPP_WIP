//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_UNLOCKABLEABILITY_H
#define VIIIARCHIVE_UNLOCKABLEABILITY_H
#include <cstdint>
#include <iostream>
#include "AbilitiesT.h"
#include "UnlockerT.h"
namespace OpenVIII::Kernel {
struct UnlockableAbility
{
private:
  UnlockerT unlocker_{};
  std::uint8_t unknown_{};
  AbilitiesT ability_{};

public:
  [[nodiscard]] auto Unlocker() const noexcept { return unlocker_; }
  [[nodiscard]] auto unknown() const noexcept { return unknown_; }
  [[nodiscard]] auto Ability() const noexcept { return ability_; }
  friend auto &operator<<(std::ostream &os, const UnlockableAbility &ua)
  {
    return os << '{' << static_cast<std::uint32_t>(ua.Unlocker()) << ", " << static_cast<std::uint32_t>(ua.unknown())
              << ", " << static_cast<std::uint32_t>(ua.Ability()) << '}';
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_UNLOCKABLEABILITY_H
