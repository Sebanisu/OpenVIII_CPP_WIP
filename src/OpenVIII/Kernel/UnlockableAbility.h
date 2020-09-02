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

#ifndef VIIIARCHIVE_UNLOCKABLEABILITY_H
#define VIIIARCHIVE_UNLOCKABLEABILITY_H
#include <cstdint>
#include <iostream>
#include "AbilitiesT.h"
#include "UnlockerT.h"
namespace open_viii::kernel {
struct UnlockableAbility
{
private:
  UnlockerT m_unlocker{};
  std::uint8_t m_unknown{};
  AbilitiesT m_ability{};

public:
  [[nodiscard]] auto unlocker() const noexcept { return m_unlocker; }
  [[nodiscard]] auto unknown() const noexcept { return m_unknown; }
  [[nodiscard]] auto ability() const noexcept { return m_ability; }
  friend auto &operator<<(std::ostream &os, const UnlockableAbility &ua)
  {
    return os << '{' << static_cast<std::uint32_t>(ua.unlocker()) << ", " << static_cast<std::uint32_t>(ua.unknown())
              << ", " << static_cast<std::uint32_t>(ua.ability()) << '}';
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_UNLOCKABLEABILITY_H
