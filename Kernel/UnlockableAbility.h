//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_UNLOCKABLEABILITY_H
#define VIIIARCHIVE_UNLOCKABLEABILITY_H
#include <cstdint>
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
  [[nodiscard]] auto Unknown() const noexcept { return unknown_; }
  [[nodiscard]] auto Ability() const noexcept { return ability_; }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_UNLOCKABLEABILITY_H
